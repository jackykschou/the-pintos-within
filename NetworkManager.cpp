#include "NetworkManager.h"

namespace pt = boost::posix_time;

NetworkManager::NetworkManager()
{
	state = NetworkStateReady;

	heartbeat = new HeartbeatPacket();
	vital     = new VitalPacket();
	particle  = new ParticlePacket();

	player_id = -1;
	num_player = 0;
	_lastHeartbeat = NULL;


	if (SDLNet_Init() < 0) {
		fprintf(stderr, "SDLNet_Init: %s\n", SDLNet_GetError());
	}
}

 NetworkManager::~NetworkManager()
 {
 	delete heartbeat;
	delete vital;
	delete particle;
	if (_lastHeartbeat) free(_lastHeartbeat);
 }

void NetworkManager::receiveChat(ChatPacket* packet)
{
	LOG("" << packet->playerId << " SAYS: " << packet->message);

	// show it in the related gui snippet
}

void NetworkManager::sendVital()
{
	send(&vital->info, sizeof(VitalInfo), true);
	vital->clear();
}

void NetworkManager::sendParticle()
{
	send(&particle->info, sizeof(ParticleInfo), true);
	particle->clear();
}

void NetworkManager::receiveHeartbeat(HeartBeatInfo* info)
{
	if (info->player_id == NetworkManager::instance()->player_id)
		return;

	if(GameState::instance()->players[info->player_id])
		heartbeat->updatePlayer(info, GameState::instance()->players[info->player_id]);
}

void NetworkManager::receiveVital(VitalInfo* info)
{
	if (info->player_id == NetworkManager::instance()->player_id)
		return;

	vital->updatePacket(info);
}

void NetworkManager::receiveParticle(ParticleInfo* info)
{
	if (info->player_id == NetworkManager::instance()->player_id)
		return;

	particle->updateParticles(info);
}

void NetworkManager::startServer() {
	LOG("Starting server...");
	
	state = NetworkStateServer;

	server = new GameServer(GAME_PORT);
	if (server->start() < 0) {
		LOG("ERROR: SERVER FAILED TO START.");
		return;
	}

	//GUIManager::instance()->showGameOverMenu();
}

void NetworkManager::startClient(const char* host) {
	LOG("Starting client...");

	state = NetworkStateClient;

	client = new GameClient(host, GAME_PORT);
	if (client->connect() < 0) {
		LOG("ERROR: CLIENT FAILED TO CONNECT.");
		return;
	}

	//GUIManager::instance()->hideGameOverMenu();
	//GUIManager::instance()->showWaitingMenu();
}

void NetworkManager::startClientDiscovery() {
	LOG("Starting client...");

	state = NetworkStateClient;

	client = new GameClient(NULL, GAME_PORT);
	client->startListeningForAdvertisements();
}

void NetworkManager::update() 
{
	if (!isActive()) return;
	
	if (GameState::instance()->isRunning()) {
		broadcastHeartbeat();
	}

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

// sends game state to every client every HEARTBEAT_MAX_DELAY milliseconds
void NetworkManager::broadcastHeartbeat() {
	pt::ptime now = pt::microsec_clock::local_time();
	pt::time_duration diff;

	if (_lastHeartbeat) {
		diff = now - *_lastHeartbeat;
	}

	if (!_lastHeartbeat || diff.total_milliseconds() > HEARTBEAT_MAX_DELAY) {
		// SEND IT HERE
		send(&(heartbeat->info), sizeof(HeartBeatInfo), false);

		if (!_lastHeartbeat) {
			_lastHeartbeat = (pt::ptime*)malloc(sizeof(pt::ptime));
		}

		*_lastHeartbeat = now;
	}
}

void NetworkManager::send(void* data, int size, bool ack)
{
	if (!isActive()) return;
	if (isServer())
	{
		server->broadcastData(data, size, ack);
	}
	else
	{
		client->sendData(data, size, ack);
	}
}

void NetworkManager::changeId(uint32_t id)
{
	player_id = id;

	heartbeat->info.player_id = id;
	vital->info.player_id = id;
	particle->info.player_id = id;
}


