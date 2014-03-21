#include "GameClient.h"
#include "GameState.h"

GameClient::GameClient(char* host, int port) {
	_host = (char*)malloc(strlen(host));
	strcpy(_host, host);

	_port = port;

	state = GameClientReady;

	_tmpSendPacket = SDLNet_AllocPacket(4096);
	_tmpRecvPacket = SDLNet_AllocPacket(4096);
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

	joinGame();

	return 0; 
}

int GameClient::joinGame() {
	printf("Sending join game request...");

	state = GameClientRunning;

	_tmpSendPacket->address.host = _srvadd.host;
	_tmpSendPacket->address.port = _srvadd.port;
	_tmpSendPacket->data = (unsigned char*)"j";
	_tmpSendPacket->len = strlen((char*)_tmpSendPacket->data) + 1;
	SDLNet_UDP_Send(_socket, 0, _tmpSendPacket);

	return 0;
}

void GameClient::update() {
	consumePackets();
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

void GameClient::processPacket(UDPpacket* packet) {

#ifdef DEBUG
	printf("UDP Packet incoming\n");
	printf("\tChan:    %d\n", packet->channel);
	printf("\tData:    %s\n", (char *)packet->data);
	printf("\tLen:     %d\n", packet->len);
	printf("\tMaxlen:  %d\n", packet->maxlen);
	printf("\tStatus:  %d\n", packet->status);
	printf("\tAddress: %x %x\n", packet->address.host, packet->address.port);
#endif

	char packetType = ((char*)packet->data)[0];
	printf("PacketType: %c\n", packetType);

	switch (packetType) {
		case 'k':
			handleJoinAckPacket(packet);
			break;
		case 's':
			handleGameStartPacket(packet);
			break;
		case 'h':
			handleHeartbeatPacket(packet);
			break;
	}
}

void GameClient::handleJoinAckPacket(UDPpacket *packet) {
	LOG("SUCCESSFULLY JOINED SERVER");
}

void GameClient::handleGameStartPacket(UDPpacket *packet) {
	LOG("STARTING GAME.");
	GUIManager::instance()->hideWaitingMenu();
	GameState::instance()->reset();
	GameState::instance()->start();
}

void GameClient::handleHeartbeatPacket(UDPpacket *packet) {
	LOG("RECEIVED HEARTBEAT PACKET");
}
