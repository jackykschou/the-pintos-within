#include "NetworkManager.h"

#define GAME_PORT 5555

NetworkManager::NetworkManager()
{
	state = NetworkStateReady;

	heartbeat = new HeartbeatPacket();
	vital = new VitalPacket();
	particle = new ParticlePacket();

	player_id = -1;
}

 NetworkManager::~NetworkManager()
 {
 	delete heartbeat;
	delete vital;
	delete particle;
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
	vital->clear();
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
	particle->clear();
}

void NetworkManager::receiveHeartbeat(HeartBeatInfo* info)
{
	if(info->player_id == NetworkManager::instance()->player_id) 
		return;

	if(GameState::instance()->players[info->player_id])
		heartbeat->updatePlayer(info, GameState::instance()->players[info->player_id]);
}

void NetworkManager::receiveVital(VitalInfo* info)
{
	if(info->player_id == NetworkManager::instance()->player_id)
		return;

	vital->updatePacket(info);
}

void NetworkManager::receiveParticle(ParticleInfo* info)
{
	if(info->player_id == NetworkManager::instance()->player_id)
		return;

	particle->updateParticles(info);
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

	if (isServer()) {
		server->update();
	} else if (isClient()) {
		client->update();
	}

	heartbeat->clear();
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

void NetworkManager::changeId(uint32_t id)
{
	player_id = id;

	heartbeat->info.player_id = id;
	vital->info.player_id = id;
	particle->info.player_id = id;
}


