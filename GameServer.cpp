#include "GameServer.h"
#include "GameState.h"

#include "HeartbeatPacket.h"
#include "VitalPacket.h"
#include "ParticlePacket.h"

namespace pt = boost::posix_time;

GameServer::GameServer(int port) {
	state  = GameServerReady;
	_port      = port;
	_tmpSendPacket = SDLNet_AllocPacket(4096);
	_tmpRecvPacket = SDLNet_AllocPacket(4096);
	_lastHeartbeat = NULL;
}

// starts the web server (opens UDP port 5555)
int GameServer::start() {
	if (state != GameServerReady) return -1;

	state = GameServerRunning;

	if (SDLNet_Init() < 0) {
		fprintf(stderr, "SDLNet_Init: %s\n", SDLNet_GetError());
		return -1;
	}

	if (!(_socket = SDLNet_UDP_Open(_port))) {
		fprintf(stderr, "SDLNet_UDP_Open: %s\n", SDLNet_GetError());
		return -1;
	}

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

	// send a heartbeat if necessary
	if (GameState::instance()->isRunning()) {
		broadcastHeartbeat();
	}
}

// sends a single packet to a single client
void GameServer::sendPacketToClient(UDPpacket* packet, IPaddress* ip) {
	printf("Sending response packet to %x:%x..\n", ip->host, ip->port);

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

// broadcasts a single packet to a bunch of clients
void GameServer::broadcastPacket(UDPpacket *packet) {
	for (int i = 0; i < _clients.size(); i++) {
		IPaddress ip = _clients.at(i);
		sendPacketToClient(packet, &ip);
	}
}

// broadcasts a single chunk of data to a bunch of clients
// this method can be used for binary or cstring (NULL terminated) buffer
void GameServer::broadcastData(void* data, int len) {
	_tmpSendPacket->data = (unsigned char*)data;
	_tmpSendPacket->len  = len+1;
	broadcastPacket(_tmpSendPacket);
}

// broadcasts a single chunk of data to a bunch of clients
// this method can ONLY be used if data is a cstring (NULL terminated) buffer
void GameServer::broadcastData(const char* data) {
	_tmpSendPacket->data = (unsigned char*)data;
	_tmpSendPacket->len  = strlen(data)+1;
	broadcastPacket(_tmpSendPacket);
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

// This is the "meat" of the packet processing logic in GameServer
// Requests are dished out based on their first byte
void GameServer::processPacket(UDPpacket *packet) {

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
		case 'j':
			// JOIN request adds a character to the game
			handleJoinPacket(packet);
			break;
		case 'e':
			// EVENT request injects an event on a linked character
			// XXX
			break;
	}
}

// when a client joins, we need to ACK back that it succeeded
void GameServer::handleJoinPacket(UDPpacket *packet) {
	IPaddress ip;

	printf("CLIENT %x %d JOINED\n", packet->address.host, packet->address.port);

	_clients.push_back(packet->address);

	_tmpSendPacket->data = (unsigned char*)"k";
	_tmpSendPacket->len = strlen((char*)_tmpSendPacket->data) + 1;

	memcpy(&ip, &(packet->address), sizeof(IPaddress));

	printf("Sending response packet..\n");

	sendPacketToClient(_tmpSendPacket, &ip);

	// the user can now start the game
	GUIManager::instance()->hideWaitingMenu();
	GUIManager::instance()->showGameOverMenu();
}

// sends GAME START event to every client
void GameServer::broadcastGameStart() {
	broadcastData("s");
}

// sends game state to every client every HEARTBEAT_MAX_DELAY milliseconds
void GameServer::broadcastHeartbeat() {
	pt::ptime now = pt::microsec_clock::local_time();
	pt::time_duration diff;

	if (_lastHeartbeat) {
		diff = now - *_lastHeartbeat;
	}

	if (!_lastHeartbeat || diff.total_milliseconds() > HEARTBEAT_MAX_DELAY) {
		LOG("SENDING HEARTBEAT PACKET");
		broadcastData("h");

		if (!_lastHeartbeat) {
			_lastHeartbeat = (pt::ptime*)malloc(sizeof(pt::ptime));
		}

		*_lastHeartbeat = now;
	}
}
