#include "GameClient.h"
#include "GameState.h"

GameClient::GameClient(char* host, int port) {
	// copy the string into a new chunk of memory :)
	_host = (char*)malloc(strlen(host)+1);
	strcpy(_host, host);

	_port          = port;
	state          = GameClientReady;
	_tmpSendPacket = SDLNet_AllocPacket(4096);
	_tmpRecvPacket = SDLNet_AllocPacket(4096);
	_ackBuffer     = new AckBuffer();
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

// sends a chunk of data in a UDP packet to the host
void GameClient::sendData(void* data, int len, bool ack, AckId id) {
	// place the rest of the data
	memcpy(_tmpSendPacket->data+sizeof(AckPacket), data, len);
	_tmpSendPacket->len = len+sizeof(AckPacket);

	// we will be shoving our ACK on top like a baller
	AckPacket ackPack;
	ackPack.ackRequired = ack;
	ackPack.isResponse  = false;

	// inject the ACK info if necessary
	if (ack) {
		if (id < 0) {
			ackPack.id = _ackBuffer->injectAck(_tmpSendPacket, _srvadd);
		} else {
			ackPack.id = id;
		}
	}

	// shove the ACK on top!
	memcpy(_tmpSendPacket->data, &ackPack, sizeof(AckPacket));

	if (SDLNet_UDP_Send(_socket, 0, _tmpSendPacket) < 0) {
		fprintf(stderr, "SDLNet_UDP_Send: %s\n", SDLNet_GetError());
	}
}

int GameClient::joinGame() {
	printf("Sending join game request...");

	state = GameClientRunning;
	sendData((void*)"j", 2, true);

	return 0;
}

void GameClient::update() {
	consumePackets();
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
		Ack* ack = iter->second;
		if (ack->isExpired()) {
			LOG("ACK EXPIRED. RESENDING REQUEST.");
			sendData(ack->packetData, ack->packetLen, true, ack->id);
			ack->reset();
		}
	}
}

void GameClient::handleGameStartPacket(UDPpacket* packet) {
	LOG("STARTING GAME.");
	GUIManager::instance()->hideWaitingMenu();
	GameState::instance()->reset();
	GameState::instance()->start();
}

void GameClient::handleHeartbeatPacket(UDPpacket* packet) {
	LOG("RECEIVED HEARTBEAT PACKET");
}


// This is the "meat" of the packet processing logic in GameServer
void GameClient::processPacket(UDPpacket* packet) {

#ifdef DEBUG
	printf("UDP Packet incoming\n");
	printf("\tChan:    %d\n", packet->channel);
	printf("\tData:    %s\n", (char*)packet->data);
	printf("\tLen:     %d\n", packet->len);
	printf("\tMaxlen:  %d\n", packet->maxlen);
	printf("\tStatus:  %d\n", packet->status);
	printf("\tAddress: %x %x\n", packet->address.host, packet->address.port);
#endif

	AckPacket* ackPacket = (AckPacket*)packet->data;
	void* packetData = packet->data+sizeof(AckPacket);
	char packetType = ((char*)packetData)[0];
	printf("PacketType: %c\n", packetType);

	// deal with ACKs immediately
	if (ackPacket->isResponse) {
		// expire the ACK
		_ackBuffer->forgetAck(ackPacket->id);
		LOG("ACK RECEIVED BY CLIENT.");
		return;
	} else if (ackPacket->ackRequired) {
		// fire off the ACK!
		AckPacket response;
		response.isResponse = true;
		response.ackRequired = false;
		response.id = ackPacket->id;
		sendData(&response, sizeof(AckPacket), &(packet->address), false);
		LOG("ACK REPLIED BY CLIENT.");
	}

	switch (packetType) {
		case 's':
			handleGameStartPacket(packet);
			break;
		case 'h':
			handleHeartbeatPacket(packet);
			break;
	}
}