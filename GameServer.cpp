#include "GameServer.h"

int backgroundThread(void *ptr);

GameServer::GameServer(int port) {
	this->state = GameServerReady;
	this->port  = port;
}

void GameServer::start() {
	if (state != GameServerReady) return;

	state = GameServerRunning;

	if (SDLNet_Init() < 0) {
		fprintf(stderr, "SDLNet_Init: %s\n", SDLNet_GetError());
		exit(EXIT_FAILURE);
	}

	if (!(_socket = SDLNet_UDP_Open(server->port))) {
		fprintf(stderr, "SDLNet_UDP_Open: %s\n", SDLNet_GetError());
		exit(EXIT_FAILURE);
	}
}

void GameServer::stop() {
	if (state != GameServerRunning) return;
	state = GameServerStopped;
}

// Packet processing is done here
void GameServer::update() {
	consumePackets();
}

void GameServer::sendHeartbeat() {
	for (int i = 0; i < _clients.size(); i++) {
		IPaddress a = _clients.at(i);
		// send packet to address a
	}
}

void GameServer::consumePackets() {
	if (server->state != GameServerRunning) {
		return -1;
	}

	/* Make space for the next packet */
	if (!(p = SDLNet_AllocPacket(512)))
	{
		fprintf(stderr, "SDLNet_AllocPacket: %s\n", SDLNet_GetError());
		return -1;
	}

	/* Consume all available packets */
	while (SDLNet_UDP_Recv(_socket, p)) {
		printf("UDP Packet incoming\n");
		printf("\tChan:    %d\n", p->channel);
		printf("\tData:    %s\n", (char *)p->data);
		printf("\tLen:     %d\n", p->len);
		printf("\tMaxlen:  %d\n", p->maxlen);
		printf("\tStatus:  %d\n", p->status);
		printf("\tAddress: %x %x\n", p->address.host, p->address.port);
		processPacket(p);
	}
}

void GameServer::processPacket(UDPpacket *packet) {
	switch (((char*)packet->data)[0]) {
		case 'j':
			// JOIN request adds a character to the game
			_clients.push_back(packet->address);
			break;
		case 'e':
			// EVENT request injects an event on a linked character
			// XXX
			break;
	}
}
