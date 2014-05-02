#include "GameServer.h"
#include "GameState.h"
#include "NetworkManager.h"
#include "HeartbeatPacket.h"
#include "VitalPacket.h"
#include "ParticlePacket.h"
#include "GuiManager.h"

namespace pt = boost::posix_time;

GameServer::GameServer(int port) 
{
	state          = GameServerReady;
	_port          = port;
	_tmpSendPacket = SDLNet_AllocPacket(4096);
	_tmpRecvPacket = SDLNet_AllocPacket(4096);
	_ackBuffer     = new AckBuffer();
	_hostname      = NULL;
	_multicastDebouncer = new Debouncer(1000, []() {
		if (!GameState::instance()->isRunning()) {
			NetworkManager::instance()->server->sendAdvertisement();
		}
	});
}

GameServer::~GameServer() {
	if (_socket) {
		SDLNet_UDP_Close(_socket);
		_socket = NULL;
	}
	SDLNet_FreePacket(_tmpSendPacket);
	SDLNet_FreePacket(_tmpRecvPacket);
	delete _ackBuffer;
	delete _multicastDebouncer;
	if (_hostname) free(_hostname);
}

// starts the web server (opens UDP port 5555)
int GameServer::start() 
{
	GameState::instance()->num_player = 1;

	if (state != GameServerReady) return -1;

	NetworkManager::instance()->changeId(0);

	state = GameServerRunning;

	if (!(_socket = SDLNet_UDP_Open(_port))) {
		fprintf(stderr, "SDLNet_UDP_Open: %s\n", SDLNet_GetError());
		return -1;
	}

	if (SDLNet_ResolveHost(&_udpBroadcastAddress, "255.255.255.255", DISCOVERY_PORT) == -1) {
		fprintf(stderr, "Failed to resolve broadcast address: %s\n", SDLNet_GetError());
		return -1;
	}

	_hostname = (char*)malloc(256);
	gethostname(_hostname, 256);

	return 0;
}

// stops the web server
void GameServer::stop() 
{
	if (state != GameServerRunning) return;
	state = GameServerStopped;
}

// called from the render loop
void GameServer::update() 
{
	consumePackets();
	_multicastDebouncer->run();
}

// sends a single packet to a single client
void GameServer::sendPacketToClient(UDPpacket* packet, IPaddress* ip) {
	// printf("Sending response packet to %x:%x..\n", ip->host, ip->port);

	// unbind from our previous client
	SDLNet_UDP_Unbind(_socket, 0);

	// this is kinda dumb. i bind to a new channel on every send
	// because i am too lazy to do manual packet address fixup :)

	if (SDLNet_UDP_Bind(_socket, 0, ip) < 0) {
		fprintf(stderr, "SDLNet_UDP_Send: %s\n", SDLNet_GetError());
	}

	if (SDLNet_UDP_Send(_socket, 0, _tmpSendPacket) < 0) {
		fprintf(stderr, "SDLNet_UDP_Send: %s\n", SDLNet_GetError());
	}
}


  // this puts an AckHeader struct above the data in the packet, and adds
  // the request to the ack buffer if necessary.
void GameServer::putDataIntoPacket(UDPpacket* p, void* data, int len, IPaddress* ip,
									bool ack, AckId id, bool isResponse) {
	// copy the data after the ack packet
	memcpy(p->data+MEMALIGNED_SIZE(AckHeader), data, len);
	p->len = len+MEMALIGNED_SIZE(AckHeader);

	// we will be shoving our ACK on top like a baller
	AckHeader ackPack;
	ackPack.ackRequired = ack;
	ackPack.isResponse  = isResponse;
	// inject the ACK info if necessary
	if (ack && id == 0) {
		ackPack.id = _ackBuffer->injectAck(p, *ip);
	} else {
		ackPack.id = id;
	}
	// shove the ACK on top!
	memcpy(p->data, &ackPack, MEMALIGNED_SIZE(AckHeader));
}

// Adds the ACK header and sends to client
void GameServer::sendDataToClient(void* data, int len, IPaddress* ip, bool ack,
								  AckId id, bool isResponse) {
	putDataIntoPacket(_tmpSendPacket, data, len, ip, ack, id, isResponse);
	sendPacketToClient(_tmpSendPacket, ip);
}

// broadcasts a single chunk of data to a bunch of clients
// this method can be used for binary or cstring (NULL terminated) buffer
void GameServer::broadcastData(void* data, int len, bool ack) {
	for (int i = 0; i < _clients.size(); i++) {
		IPaddress ip = _clients.at(i);
		sendDataToClient(data, len, &ip, ack);
	}
}

void GameServer::resendExpiredAcks() {
	std::map<AckId, Ack*>::iterator iter;
	for (iter = _ackBuffer->buffer.begin(); iter != _ackBuffer->buffer.end();) {
		Ack* ack = iter->second;
		if (ack->isExpired()) {
			// LOG("ACK EXPIRED. RESENDING REQUEST.");
			sendDataToClient(ack->packetData, ack->packetLen, &ack->address, true, ack->id);
			ack->reset();
		} else {
			iter++;
		}
	}
}

// nom noms any available UDP packets from the wire
void GameServer::consumePackets() {
	if (state != GameServerRunning) {
		return;
	}

	// Consume all available packets in the buffer
	while (SDLNet_UDP_Recv(_socket, _tmpRecvPacket)) {
		processPacket(_tmpRecvPacket);
	}
}

// sends a UDP broadcast to clients in the same NAT subnet
// that we are running a LAN game here.
void GameServer::sendAdvertisement() {
	LOG("BROADCASTING ADVERTISEMENT");
	ServerAdvertisement ad;
	strncpy(ad.magic, DISCOVERY_SIGNATURE, 5);
	strncpy(ad.name, _hostname, 256);
	strncpy(ad.description, "JOES COOL GAME", 256);
	sendDataToClient(&ad, sizeof(ServerAdvertisement), &_udpBroadcastAddress, false);
}

void GameServer::broadcastGameStart() {
	char c = GAMESTART;
	broadcastData(&c, 1, true);
}

// This is the "meat" of the packet processing logic in GameServer
// Requests are dished out based on their first byte
void GameServer::processPacket(UDPpacket* packet) {

	AckHeader* ackHeader = (AckHeader*)packet->data;
	void* packetData = packet->data+MEMALIGNED_SIZE(AckHeader);
	char packetType = ((char*)packetData)[0];
	// printf("PacketType: %c\n", packetType);

	if (ackHeader->isResponse) {
		// woot. expire our ACK.
		_ackBuffer->forgetAck(ackHeader->id);
		// LOG("ACK RECEIVED BY HOST: "<<ackHeader->id);
		return;
	} else if (ackHeader->ackRequired) {
		// fire off the ACK!
		sendDataToClient((void*)"A", 2, &(packet->address), false, ackHeader->id, true);
		// LOG("ACK REPLIED BY HOST: "<<ackHeader->id);
	}

	switch (packetType) {
		case JOINGAME:
			// JOIN request adds a character to the game
			handleJoinPacket(packet, packetData);
			break;
		case HEARTBEATPACK:
			HeartBeatInfo* hinfo;
			hinfo =  (HeartBeatInfo*) packetData;
			NetworkManager::instance()->receiveHeartbeat(hinfo);
			broadcastData(hinfo, sizeof(HeartBeatInfo), false);
			break;
		case CHATPACK:
			ChatPacket* chat;
			chat = (ChatPacket*)packetData;
			NetworkManager::instance()->receiveChat(chat);
			broadcastData(chat, sizeof(ChatPacket), true);
			break;
		case TAKEDAMAGE:
			PlayerDamageInfo* damage_info;
			damage_info =  (PlayerDamageInfo*) packetData;
			NetworkManager::instance()->vital->receiveDamage(damage_info);
			broadcastData(damage_info, sizeof(PlayerDamageInfo), true);
			break;
		case PLAYER_DIE:
			PlayerDieInfo* player_die_info;
			player_die_info =  (PlayerDieInfo*) packetData;
			NetworkManager::instance()->vital->receivePlayerDie(player_die_info);
			break;
		case WEAPON_CHANGE:
			ChangeWeaponInfo* weapon_change_info;
			weapon_change_info =  (ChangeWeaponInfo*) packetData;
			NetworkManager::instance()->vital->receiveChangeWeapon(weapon_change_info);
			broadcastData(weapon_change_info, sizeof(ChangeWeaponInfo), true);
			break;
		case WEAPON_SPAWN:
			WeaponSpawnInfo* weapon_spawn_info;
			weapon_spawn_info =  (WeaponSpawnInfo*) packetData;
			NetworkManager::instance()->vital->receiveSpawnWeapon(weapon_spawn_info);
			broadcastData(weapon_spawn_info, sizeof(WeaponSpawnInfo), true);
			break;
		case BLOOD:
			BloodInfo* blood_info;
			blood_info =  (BloodInfo*) packetData;
			NetworkManager::instance()->particle->receiveBlood(blood_info);
			broadcastData(blood_info, sizeof(BloodInfo), true);
			break;
		case DUST:
			DustInfo* dust_info;
			dust_info =  (DustInfo*) packetData;
			NetworkManager::instance()->particle->receiveDust(dust_info);
			broadcastData(dust_info, sizeof(DustInfo), true);
			break;
		case BLASTER_EXPLODE:
			BlasterExplodeInfo* blaster_info;
			blaster_info =  (BlasterExplodeInfo*) packetData;
			NetworkManager::instance()->particle->receiveBlasterExplosion(blaster_info);
			broadcastData(blaster_info, sizeof(BlasterExplodeInfo), true);
			break;
		case PLAY_FIRE_SOUND:
			PlayFireSoundInfo* fire_info;
			fire_info =  (PlayFireSoundInfo*) packetData;
			NetworkManager::instance()->vital->receivePlayFireSound(fire_info);
			broadcastData(fire_info, sizeof(PlayFireSoundInfo), true);
			break;
		case CHANGE_PINTO:
			ChangePintoInfo* change_pinto_info;
			change_pinto_info =  (ChangePintoInfo*) packetData;
			NetworkManager::instance()->vital->receiveChangePinto(change_pinto_info);
			broadcastData(change_pinto_info, sizeof(ChangePintoInfo), true);
			break;
		case INCREASE_SCORE:
			IncreaseScoreInfo* score_info;
			score_info =  (IncreaseScoreInfo*) packetData;
			NetworkManager::instance()->vital->receiveIncreaseScore(score_info);
			broadcastData(score_info, sizeof(IncreaseScoreInfo), true);
	}
}

// when a client joins, we need to ACK back that it succeeded
void GameServer::handleJoinPacket(UDPpacket *packet, void* data) {
	IPaddress ip;

	printf("CLIENT %x %d JOINED\n", packet->address.host, packet->address.port);

	JoinRequestPacket* join;
	join = (JoinRequestPacket*)data;
	LOG("PLAYER'S NAME IS " << join->name);

	_clients.push_back(packet->address);
	int id = _clients.size();
	GameState::instance()->num_player++;

	int len = strlen(join->name);
	if (len > 14) len = 14;
	char c = '1';
	while (GameState::instance()->nameIsTaken(join->name)) {
		LOG("NAME IS TAKEN!!!!");
		join->name[len] = c++;
		join->name[len+1] = '\0';
		LOG("NEW NAME= " << join->name);
	}

	GameState::instance()->setPlayerName(id, std::string(join->name));

	memcpy(&ip, &(packet->address), sizeof(IPaddress));

	PlayerIdInfo info;
	info.type = ASSIGNPLAYERID;
	info.player_id = id;

	char x = ASSIGNPLAYERID;
	sendDataToClient(&info, sizeof(PlayerIdInfo), &ip, true);

	// send a PlayerJoinPacket to the new client for every other player
	for(std::map<int,bool>::iterator iter = GameState::instance()->playerConnections.begin();
		iter != GameState::instance()->playerConnections.end(); ++iter) {
		int i = iter->first;
		PlayerJoinPacket p;
		p.type = PLAYER_JOIN;
		p.playerId = i;
		strncpy(p.name, GameState::instance()->getPlayerName(i).c_str(), 16);
		sendDataToClient(&p, sizeof(PlayerJoinPacket), &ip, true);
	}

	// broadcasts a PlayerJoinPacket to the other clients about the new client
	PlayerJoinPacket p;
	p.type = PLAYER_JOIN;
	p.playerId = id;
	strncpy(p.name, join->name, 16);
	broadcastData(&p, sizeof(PlayerJoinPacket), true);

	LOG("Assigning player id: " << id);
        GuiManager::instance()->EnableStart();
}
