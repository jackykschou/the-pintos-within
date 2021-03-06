#include "GameClient.h"
#include "GameState.h"
#include "NetworkManager.h"
#include "HeartbeatPacket.h"
#include "VitalPacket.h"
#include "ParticlePacket.h"
#include "GuiManager.h"

GameClient::GameClient(const char* host, int port) {
	// copy the string into a new chunk of memory :)
	if (host) {
		_host = (char*)malloc(strlen(host)+1);
		strcpy(_host, host);
	} else {
		_host = NULL;
	}

	_port          = port;
	state          = GameClientReady;
	_tmpSendPacket = SDLNet_AllocPacket(4096);
	_tmpRecvPacket = SDLNet_AllocPacket(4096);
	_ackBuffer     = new AckBuffer();
	_discoverySocket = NULL;
}

GameClient::~GameClient() {
	SDLNet_FreePacket(_tmpSendPacket);
	SDLNet_FreePacket(_tmpRecvPacket);
	stopListeningForAdvertisements();
	delete _ackBuffer;
}

int GameClient::connect() {
	if (state != GameClientReady) return -1;

	state = GameClientConnecting;

	/* Resolve server name  */
	if (SDLNet_ResolveHost(&_srvadd, _host, _port) == -1)
	{
		fprintf(stderr, "SDLNet_ResolveHost(%s %d): %s\n", _host, _port, SDLNet_GetError());
		return -1;
	}

	/* Open a socket on random port */
	if (!(_socket = SDLNet_UDP_Open(0)))
	{
		fprintf(stderr, "SDLNet_UDP_Open: %s\n", SDLNet_GetError());
		return -1;
	}

	if (SDLNet_UDP_Bind(_socket, 0, &_srvadd) < 0)
	{
		fprintf(stderr, "SDLNet_UDP_Bind: %s\n", SDLNet_GetError());
		return 1;
	}

	// yup.
	_tmpSendPacket->address.host = _srvadd.host;
	_tmpSendPacket->address.port = _srvadd.port;

	joinGame();

	return 0; 
}

void GameClient::startListeningForAdvertisements() {
        if(state!=GameClientReady){return;}
	LOG("LISTENING FOR ADS");
	state = GameClientDiscovering;
	if (!(_discoverySocket = SDLNet_UDP_Open(DISCOVERY_PORT))) {
		fprintf(stderr, "SDLNet_UDP_Open: %s\n", SDLNet_GetError());
		_discoverySocket = NULL;
		return;
	}
}

void GameClient::stopListeningForAdvertisements() {
        if(state!=GameClientDiscovering){return;}
	state = GameClientReady;
	if (_discoverySocket) SDLNet_UDP_Close(_discoverySocket);
	_discoverySocket = NULL;
}

// sends a chunk of data in a UDP packet to the host
void GameClient::sendData(void* data, int len, bool ack, AckId id, bool isResponse) {
	// place the rest of the data
	memcpy(_tmpSendPacket->data+MEMALIGNED_SIZE(AckHeader), data, len);
	_tmpSendPacket->len = len+MEMALIGNED_SIZE(AckHeader);

	// we will be shoving our ACK on top like a baller
	AckHeader ackPack;
	ackPack.ackRequired = ack;
	ackPack.isResponse  = isResponse;

	// inject the ACK info if necessary
	if (ack && id == 0) {
		ackPack.id = _ackBuffer->injectAck(_tmpSendPacket, _srvadd);
		// LOG("INJECTING ACK HEADER INTO PACKET " << ackPack.id);
	} else {
		ackPack.id = id;
	}

	// shove the ACK on top!
	memcpy(_tmpSendPacket->data, &ackPack, MEMALIGNED_SIZE(AckHeader));

	if (SDLNet_UDP_Send(_socket, 0, _tmpSendPacket) < 0) {
		fprintf(stderr, "SDLNet_UDP_Send: %s\n", SDLNet_GetError());
	}
}

int GameClient::joinGame() {
	printf("Sending join game request...");

	state = GameClientRunning;

	JoinRequestPacket request;
	request.type = JOINGAME;
	std::string name = GuiManager::instance()->GetName();
	LOG("MY NAME IS "<<name);
	strncpy(request.name, name.c_str(), sizeof(request.name));

	sendData(&request, sizeof(JoinRequestPacket), true);

	return 0;
}

void GameClient::update() {
	consumePackets();
	consumeDiscoveryPackets();
	resendExpiredAcks();
}

// let's see what packets the server has sent us
int GameClient::consumePackets() {
	if (state != GameClientRunning) {
		return -1;
	}

	while (SDLNet_UDP_Recv(_socket, _tmpRecvPacket)) {
		processPacket(_tmpRecvPacket);
	}

	return 0;
}

void GameClient::resendExpiredAcks() {
	std::map<AckId, Ack*>::iterator iter;
	for (iter = _ackBuffer->buffer.begin(); iter != _ackBuffer->buffer.end(); iter++) {
		LOG("ITERATING ACKS");
		Ack* ack = iter->second;
		if (ack->isExpired()) {
			LOG("ACK EXPIRED. RESENDING REQUEST.");
			sendData(ack->packetData, ack->packetLen, true, ack->id);
			ack->reset();
		}
	}
}

void GameClient::consumeDiscoveryPackets() {
	if (state != GameClientDiscovering || _discoverySocket == NULL) {
		return;
	}
	while (SDLNet_UDP_Recv(_discoverySocket, _tmpRecvPacket)) {
		void* packetData = _tmpRecvPacket->data+MEMALIGNED_SIZE(AckHeader);
		ServerAdvertisement* ad;
		ad = (ServerAdvertisement*)packetData;
		printf("RECEIVED SERVER DISCOVERY PACKET\n%s\n%s\n", ad->name, ad->description);
		GameState::instance()->games[ad->name]=std::make_pair(ad->description,boost::posix_time::second_clock::local_time());
	}
}

// This is the "meat" of the packet processing logic in GameServer
void GameClient::processPacket(UDPpacket* packet) {

	AckHeader* ackHeader = (AckHeader*)packet->data;
	void* packetData = packet->data+MEMALIGNED_SIZE(AckHeader);
	char packetType = ((char*)packetData)[0];

	// deal with ACKs immediately
	if (ackHeader->isResponse) {
		// expire the ACK
		_ackBuffer->forgetAck(ackHeader->id);
		LOG("ACK RECEIVED BY CLIENT.");
		return;
	} else if (ackHeader->ackRequired) {
		// fire off the ACK!
		if (GameState::instance()->isRunning()) {
			sendData((void*)"A", 2, false, ackHeader->id, true);
			LOG("ACK REPLIED BY CLIENT.");
		}
	}

	if (!GameState::instance()->isRunning()) {
		if (!(packetType == GAMESTART ||
			  packetType == ASSIGNPLAYERID ||
			  packetType == CHATPACK ||
			  packetType == PLAYER_JOIN ||
			  packetType == PING ||
			  packetType == PLAYER_DISCONNECT)) {
			return;
		}
	}

	switch (packetType) 
	{
		case PLAYERNUM:
			PlayerNumInfo* ninfo;
			ninfo = (PlayerNumInfo*) packetData;
			GameState::instance()->num_player = ninfo->num_player;
			break;
		case GAMESTART:
			GameStartPacket* start_packet;
			start_packet = (GameStartPacket*) packetData;
			GameState::instance()->game_mode = start_packet->game_mode;
			GameState::instance()->team_mode = start_packet->team_mode;
			GameState::instance()->current_map = start_packet->current_map;
			LOG("WE BE STARTIN YO!");
			if (!GameState::instance()->isRunning()) {
		    	GuiManager::instance()->Start();
			}
			break;
		case ASSIGNPLAYERID:
			PlayerIdInfo* pinfo;
			pinfo = (PlayerIdInfo*) packetData;
			NetworkManager::instance()->changeId(pinfo->player_id);
			GameState::instance()->current_map = pinfo->current_map;
			break;
		case HEARTBEATPACK:
			HeartBeatInfo* hinfo;
			hinfo =  (HeartBeatInfo*) packetData;
			NetworkManager::instance()->receiveHeartbeat(hinfo);
			break;
		case CHATPACK:
			ChatPacket* chat;
			chat = (ChatPacket*)packetData;
			NetworkManager::instance()->receiveChat(chat);
			break;
		case TAKEDAMAGE:
			PlayerDamageInfo* damage_info;
			damage_info =  (PlayerDamageInfo*) packetData;
			NetworkManager::instance()->vital->receiveDamage(damage_info);
			break;
		case PLAYER_RESPAWN:
			PlayerRespawnInfo* player_respawn_info;
			player_respawn_info =  (PlayerRespawnInfo*) packetData;
			NetworkManager::instance()->vital->receivePlayerRespawn(player_respawn_info);
			break;
		case WEAPON_CHANGE:
			ChangeWeaponInfo* weapon_change_info;
			weapon_change_info =  (ChangeWeaponInfo*) packetData;
			NetworkManager::instance()->vital->receiveChangeWeapon(weapon_change_info);
			break;
		case WEAPON_SPAWN:
			WeaponSpawnInfo* weapon_spawn_info;
			weapon_spawn_info =  (WeaponSpawnInfo*) packetData;
			NetworkManager::instance()->vital->receiveSpawnWeapon(weapon_spawn_info);
			break;
		case BLOOD:
			BloodInfo* blood_info;
			blood_info =  (BloodInfo*) packetData;
			NetworkManager::instance()->particle->receiveBlood(blood_info);
			break;
		case DUST:
			DustInfo* dust_info;
			dust_info =  (DustInfo*) packetData;
			NetworkManager::instance()->particle->receiveDust(dust_info);
			break;
		case BLASTER_EXPLODE:
			BlasterExplodeInfo* blaster_info;
			blaster_info =  (BlasterExplodeInfo*) packetData;
			NetworkManager::instance()->particle->receiveBlasterExplosion(blaster_info);
			break;
		case PLAY_FIRE_SOUND:
			PlayFireSoundInfo* fire_info;
			fire_info =  (PlayFireSoundInfo*) packetData;
			NetworkManager::instance()->vital->receivePlayFireSound(fire_info);
			break;
		case CHANGE_PINTO:
			ChangePintoInfo* change_pinto_info;
			change_pinto_info =  (ChangePintoInfo*) packetData;
			NetworkManager::instance()->vital->receiveChangePinto(change_pinto_info);
			break;
		case INCREASE_SCORE:
			IncreaseScoreInfo* score_info;
			score_info =  (IncreaseScoreInfo*) packetData;
			NetworkManager::instance()->vital->receiveIncreaseScore(score_info);
			break;
		case TIME_LEFT:
			TimeLeftInfo* time_info;
			time_info =  (TimeLeftInfo*) packetData;
			NetworkManager::instance()->vital->receiveTimeLeft(time_info);
			break;
		case PLAYER_JOIN:
			PlayerJoinPacket* p;
			p = (PlayerJoinPacket*)packetData;
			if (p->playerId != NetworkManager::instance()->player_id) {
				printf("Player %d joined, with name %s\n", p->playerId, p->name);
			}
			GameState::instance()->setPlayerName(p->playerId, p->name);
			++GameState::instance()->num_player;
			GameState::instance()->game_mode = p->game_mode;
			GameState::instance()->team_mode = p->team_mode;
			GameState::instance()->current_map = p->current_map;
			break;
		case PING:
			PingPacket ping;
			ping.type = PING;
			ping.playerId = NetworkManager::instance()->player_id;
			sendData(&ping, sizeof(PingPacket), false);
			break;
		case PLAYER_DISCONNECT:
			PlayerDisconnectPacket* disconnect;
			disconnect = (PlayerDisconnectPacket*)packetData;
			GameState::instance()->removePlayer(disconnect->playerId);
			LOG("REMOVED PALYER "<<disconnect->playerId);
			break;
		case GAME_OVER:
			GameOverPacket* gameOver;
			gameOver = (GameOverPacket*)packetData;
			GameState::instance()->stop(gameOver->message);
			LOG("SHOWING GAME OVER MESSAGE "<<gameOver->message);
			break;
		case HAIR_CHANGE:
			ChangeHairInfo *hair;
			hair =  (ChangeHairInfo*) packetData;
			NetworkManager::instance()->vital->receiveChangeHair(hair);
			break;
	}
}
