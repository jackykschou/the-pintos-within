#include "NetworkManager.h"

#define GAME_PORT 5555

void NetworkManager::startServer() {
	_server = new GameServer(GAME_PORT);
}

void NetwosdsadzzrkManager::startClient() {
	_client = new GameClient();
}

bool NetworkManager::isActive() {
	return state != NetworkReady;
}

bool NetworkManager::isServer() {
	return state == NetworkServer;
}

bool NetworkManager::isServer() {
	return state == NetworkClient;
}
