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
	SDLNet_FreePacket(_tmpSendPacket);
	SDLNet_FreePacket(_tmpRecvPacket);
	delete _ackBuffer;
	delete _multicastDebouncer;
	if (_hostname) free(_hostname);
}

// starts the web server (opens UDP port 5555)
int GameServer::start() 
{
	NetworkManager::instance()->num_player = 1;

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
void GameServer::stop() {
	if (state != GameServerRunning) return;
	state = GameServerStopped;
}

// called from the render loop
void GameServer::update() {
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

// broadcasts a single cstring (data->"\x00") to a bunch of clients
void GameServer::broadcastString(const char* data, bool ack) {
	broadcastData((void*)data, strlen(data)+1, ack);
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


// when a client joins, we need to ACK back that it succeeded
void GameServer::handleJoinPacket(UDPpacket *packet) {
	IPaddress ip;

	printf("CLIENT %x %d JOINED\n", packet->address.host, packet->address.port);

	_clients.push_back(packet->address);
	int id = _clients.size();
	++(NetworkManager::instance()->num_player);

	memcpy(&ip, &(packet->address), sizeof(IPaddress));

	PlayerIdInfo info;
	info.type = ASSIGNPLAYERID;
	info.player_id = id;

	char x = ASSIGNPLAYERID;
	sendDataToClient(&info, sizeof(PlayerIdInfo), &ip, true);

	LOG("Assigning player id: " << id);
        GuiManager::instance()->EnableStart();
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

// This is the "meat" of the packet processing logic in GameServer
// Requests are dished out based on their first byte
void GameServer::processPacket(UDPpacket* packet) {

#ifdef DEBUG
	// printf("UDP Packet incoming\n");
	// printf("\tChan:    %d\n", packet->channel);
	// printf("\tData:    %s\n", (char*)packet->data);
	// printf("\tLen:     %d\n", packet->len);
	// printf("\tMaxlen:  %d\n", packet->maxlen);
	// printf("\tStatus:  %d\n", packet->status);
	// printf("\tAddress: %x %x\n", packet->address.host, packet->address.port);
#endif

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
			handleJoinPacket(packet);
			break;
		case VITALPACK:
			VitalInfo* vinfo;
			vinfo =  (VitalInfo*) packetData;
			NetworkManager::instance()->receiveVital(vinfo);
			broadcastData(vinfo, sizeof(VitalInfo), true);
			break;
		case HEARTBEATPACK:
			HeartBeatInfo* hinfo;
			hinfo =  (HeartBeatInfo*) packetData;
			NetworkManager::instance()->receiveHeartbeat(hinfo);
			broadcastData(hinfo, sizeof(HeartBeatInfo), false);
			break;
		case PARTICLEPACK:
			ParticleInfo* pinfo;
			pinfo =  (ParticleInfo*) packetData;
			NetworkManager::instance()->receiveParticle(pinfo);
			broadcastData(pinfo, sizeof(ParticleInfo), true);
			break;
		case CHATPACK:
			ChatPacket* chat;
			chat = (ChatPacket*)packetData;
			NetworkManager::instance()->receiveChat(chat);
			break;
	}
}
