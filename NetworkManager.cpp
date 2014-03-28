#include "NetworkManager.h"

#define GAME_PORT 5555

NetworkManager::NetworkManager()
{
	state = NetworkStateReady;

	heartbeatSend = new HeartbeatPacket();
	vitalSend = new VitalPacket();
	particleSend = new ParticlePacket();

	heartbeatReceive = new HeartbeatPacket();
	vitalReceive = new VitalPacket();
	particleReceive = new ParticlePacket();
}

 NetworkManager::~NetworkManager()
 {
 	delete heartbeatSend;
	delete vitalSend;
	delete particleSend;

	delete heartbeatReceive;
	delete vitalReceive;
	delete particleReceive;
 }

void NetworkManager::sendVital()
{
	// if(isServer())
	// {
	// 	server->broadcastData();
	// }
	// else
	// {
	// 	client->sendData();
	// }
	vitalSend->clear();
}

void NetworkManager::sendParticle()
{
	// if(isServer())
	// {
	// 	server->broadcastData();
	// }
	// else
	// {
	// 	client->sendData();
	// }
}

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

void NetworkManager::update() 
{
	if (!isActive()) return;

	vitalReceive->clear();
	particleReceive->clear();

	if (isServer()) {
		server->update();
	} else if (isClient()) {
		client->update();
	}

	heartbeatSend->clear();
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

