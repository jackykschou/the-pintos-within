#include "GameClient.h"
#include "GameState.h"

#include "NetworkManager.h"

#include "HeartbeatPacket.h"
#include "VitalPacket.h"
#include "ParticlePacket.h"

GameClient::GameClient(char* host, int port) {
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
	LOG("LISTENING FOR ADS");
	state = GameClientDiscovering;
	if (!(_discoverySocket = SDLNet_UDP_Open(DISCOVERY_PORT))) {
		fprintf(stderr, "SDLNet_UDP_Open: %s\n", SDLNet_GetError());
		return;
	}
}

void GameClient::stopListeningForAdvertisements() {
	state = GameClientReady;
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
		LOG("INJECTING ACK HEADER INTO PACKET " << ackPack.id);
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
	char x = JOINGAME;
	sendData(&x, 1, true);

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
	if (state != GameClientDiscovering) {
		return;
	}

	while (SDLNet_UDP_Recv(_discoverySocket, _tmpRecvPacket)) {
		void* packetData = _tmpRecvPacket->data+MEMALIGNED_SIZE(AckHeader);
		ServerAdvertisement* ad;
		ad = (ServerAdvertisement*)packetData;
		printf("RECEIVED SERVER DISCOVERY PACKET\n%s\n%s\n", ad->name, ad->description);
	}
}

// This is the "meat" of the packet processing logic in GameServer
void GameClient::processPacket(UDPpacket* packet) {

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

	// deal with ACKs immediately
	if (ackHeader->isResponse) {
		// expire the ACK
		_ackBuffer->forgetAck(ackHeader->id);
		LOG("ACK RECEIVED BY CLIENT.");
		return;
	} else if (ackHeader->ackRequired) {
		// fire off the ACK!
		sendData((void*)"A", 2, false, ackHeader->id, true);
		LOG("ACK REPLIED BY CLIENT.");
	}

	switch (packetType) 
	{
		case ASSIGNPLAYERID:
			PlayerIdInfo* pinfo;
			pinfo = (PlayerIdInfo*) packetData;
			NetworkManager::instance()->changeId(pinfo->player_id);
			break;
		case VITALPACK:
			VitalInfo* vinfo;
			vinfo =  (VitalInfo*) packetData;
			NetworkManager::instance()->receiveVital(vinfo);
			break;
		case HEARTBEATPACK:
			HeartBeatInfo* hinfo;
			hinfo =  (HeartBeatInfo*) packetData;
			NetworkManager::instance()->receiveHeartbeat(hinfo);
			if (!GameState::instance()->isRunning()) {
		        GameState::instance()->reset();
	            GameState::instance()->start();
	        }
			break;
		case PARTICLEPACK:
			ParticleInfo* particleinfo;
			particleinfo =  (ParticleInfo*) packetData;
			NetworkManager::instance()->receiveParticle(particleinfo);
			break;
		
	}
}
