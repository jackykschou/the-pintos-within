#include "NetworkManager.h"

#define GAME_PORT 5555

void NetworkManager::startServer() {
	LOG("Starting server...");
	
	state = NetworkStateServer;

	server = new GameServer(GAME_PORT);
	if (server->start() < 0) {
		return;
	}

	GUIManager::instance()->hideGameOverMenu();
	GUIManager::instance()->showWaitingMenu();
}

void NetworkManager::startClient(char* host) {
	LOG("Starting client...");

	state = NetworkStateClient;

	client = new GameClient(host, GAME_PORT);
	if (client->connect() < 0) {
		return;
	}

	GUIManager::instance()->hideGameOverMenu();
	GUIManager::instance()->showWaitingMenu();
}

void NetworkManager::update() {
	if (!isActive()) return;

	if (isServer()) {
		server->update();
	} else if (isClient()) {
		client->update();
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
