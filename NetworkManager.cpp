#include "NetworkManager.h"

#define GAME_PORT 5555

void NetworkManager::startServer() {
	LOG("Starting server...");

	_server = new GameServer(GAME_PORT);
	if (_server->start() < 0) {
		return;
	}

	state = NetworkStateServer;
}

void NetworkManager::startClient(char* host) {
	LOG("Starting client...");
 
	_client = new GameClient(host, GAME_PORT);
	if (_client->connect() < 0) {
		return;
	}

	state = NetworkStateClient;
}

void NetworkManager::update() {
	if (!isActive()) return;

	if (isServer()) {
		_server->update();
	} else if (isClient()) {
		_client->update();
	}
}

bool NetworkManager::isActive() {
	return state != NetworkStateReady;
}

bool NetworkManager::isServer() {
	return state == NetworkStateServer;
}

bool NetworkManager::isClient() {
	return state == NetworkStateClient;
}
