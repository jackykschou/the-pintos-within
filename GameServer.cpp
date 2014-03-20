#include "GameServer.h"

int backgroundThread(void *ptr);

GameServer::GameServer(int port) {
	this->state = GameServerReady;
	this->port  = port;
}

void GameServer::start() {
	if (state != GameServerReady) return;
	_serverThread = SDL_CreateThread(backgroundThread, (void *)this);
}

void GameServer::stop() {
	if (state != GameServerRunning) return;
	state = GameServerStopped;
}

// Packet processing is done here
void GameServer::update() {
	for (int i = 0; i < packetBuffer.size(); i++) {
		UDPpacket *packet = packetBuffer.front();
		packetBuffer.pop_front();
		processPacket(packet);
		SDLNet_FreePacket(packet);
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

void GameServer::sendHeartbeat() {
	for (int i = 0; i < _clients.size(); i++) {
		IPaddress a = _clients.at(i);
	}
}

int backgroundThread(void *serverPtr) {
	GameServer* server = (GameServer*)serverPtr;
	UDPsocket sd;       /* Socket descriptor */
	UDPpacket *p;       /* Pointer to packet memory */
	
	/* Initialize SDL_net */
	if (SDLNet_Init() < 0)
	{
		fprintf(stderr, "SDLNet_Init: %s\n", SDLNet_GetError());
		exit(EXIT_FAILURE);
	}

	/* Open a socket */
	if (!(sd = SDLNet_UDP_Open(server->port)))
	{
		fprintf(stderr, "SDLNet_UDP_Open: %s\n", SDLNet_GetError());
		exit(EXIT_FAILURE);
	}

	while (true) {
		if (server->state != GameServerRunning) {
			return -1;
		}

		/* Make space for the next packet */
		if (!(p = SDLNet_AllocPacket(512)))
		{
			fprintf(stderr, "SDLNet_AllocPacket: %s\n", SDLNet_GetError());
			return -1;
		}

		/* Shove the packet into our buffer for inspection by server */
		if (SDLNet_UDP_Recv(sd, p)) {
			printf("UDP Packet incoming\n");
			printf("\tChan:    %d\n", p->channel);
			printf("\tData:    %s\n", (char *)p->data);
			printf("\tLen:     %d\n", p->len);
			printf("\tMaxlen:  %d\n", p->maxlen);
			printf("\tStatus:  %d\n", p->status);
			printf("\tAddress: %x %x\n", p->address.host, p->address.port);
 
			server->packetBuffer.push_back(p);
		}

		/* send all outgoing packets! */
		for (int i = 0; i < server->sendBuffer.size(); i++) {
			UDPpacket *sendPacket = server->sendBuffer.front();
			server->sendBuffer.pop_front();
			if (!SDLNet_UDP_Send(sd, sendPacket->channel, sendPacket)) {
			    printf("SDLNet_UDP_Send: %s\n", SDLNet_GetError());
			    // do something because we failed to send
			    // this may just be because no addresses are bound to the channel...
			}
		}
	}

	return 1;
}
