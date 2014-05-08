#include "NetworkManager.h"
#include "ChatManager.h"
#include "PlayerCharacter.h"

namespace pt = boost::posix_time;

NetworkManager::NetworkManager()
{
	state = NetworkStateReady;

	heartbeat = new HeartbeatPacket();
	vital     = new VitalPacket();
	particle  = new ParticlePacket();

	player_id = -1;
	_lastHeartbeat = NULL;


	if (SDLNet_Init() < 0) 
	{
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
	if (packet->playerId == player_id)
		return;

	std::string name = GameState::instance()->getPlayerName(packet->playerId);
	ChatManager::instance()->addMessage(name.c_str(), packet->message);
}

void NetworkManager::sendChat(const char* msg)
{
	ChatPacket cp;
	cp.type = CHATPACK;
	cp.playerId = player_id;
	strncpy(cp.message, msg, sizeof(cp.message));
	send(&cp, sizeof(ChatPacket), true);
}

void NetworkManager::receiveHeartbeat(HeartBeatInfo* info)
{
	if (info->player_id == player_id || !GameState::instance()->isRunning())
		return;

	if(GameState::instance()->players[info->player_id])
		heartbeat->updatePlayer(info, GameState::instance()->players[info->player_id]);
}

void NetworkManager::startServer() {
	if (state == NetworkStateServer) stopServer();
	if (state == NetworkStateClient) stopClient();

	LOG("Starting server...");
	
	state = NetworkStateServer;

	server = new GameServer(GAME_PORT);
	if (server->start() < 0) {
		LOG("ERROR: SERVER FAILED TO START.");
		return;
	}
}

void NetworkManager::stopServer() {
	if (state == NetworkStateServer) {
		// kill the old server
		delete server;
		server = NULL;
		state = NetworkStateReady;
	}
}

void NetworkManager::sendGameOverPacket() {
	if (state == NetworkStateServer) {
		int max = -1;
		std::string winner;
		for(auto iter = GameState::instance()->playerScores.begin();
                 iter != GameState::instance()->playerScores.end(); ++iter)
        {
        	if (max < iter->second) {
        		max = iter->second;
        		winner = GameState::instance()->getPlayerName(iter->first);
        	}
        }

        LOG("WINNER HAS "<<max);
        bool teamModeOn = (GameState::instance()->team_mode == TEAM && GameState::instance()->game_mode != PINTO);

        for(auto iter = GameState::instance()->playerConnections.begin();
                 iter != GameState::instance()->playerConnections.end(); ++iter)
        {
        	int id = iter->first;

        	std::stringstream str;
        	if (GameState::instance()->playerScores[id] == max) {
        		str << "You win!";
        	} else {
        		if (teamModeOn) {
					str << "You lose!";
				} else {
	        		str << "You lose! " << winner << " is the winner.";
	        	}
        	}

			GameOverPacket p;
			p.type = GAME_OVER;
			strncpy(p.message, str.str().c_str(), GAME_OVER_MSG_MAX_LEN);
			p.message[GAME_OVER_MSG_MAX_LEN-1] = '\0';

			server->sendDataToClient(&p, sizeof(GameOverPacket), id, true);
		}

		std::stringstream str;
		if (GameState::instance()->playerScores[0] == max) {
			str << "You win!";
		} else {
			if (teamModeOn) {
				str << "You lose!";
			} else {
				str << "You lose! " << winner << " is the winner.";
			}
		}

		GameState::instance()->stop(str.str());
	}
}

void NetworkManager::startClient(const char* host) {
	if (state == NetworkStateServer) stopServer();
	if (state == NetworkStateClient) stopClient();

	LOG("Starting client...");

	state = NetworkStateClient;

	client = new GameClient(host, GAME_PORT);
	if (client->connect() < 0) {
		LOG("ERROR: CLIENT FAILED TO CONNECT.");
		return;
	}
}

void NetworkManager::stopClient() {
	if (state == NetworkStateClient) {
		// kill the old client
		delete client;
		client = NULL;
		state = NetworkStateReady;
	}
}

void NetworkManager::startClientDiscovery() {
	if (state == NetworkStateClient) return;

	LOG("Starting client discovery...");

	state = NetworkStateClient;

	client = new GameClient(NULL, GAME_PORT);
	client->startListeningForAdvertisements();
}

void NetworkManager::stopClientDiscovery() {
	if (state != NetworkStateClient) return;

	LOG("Stopping client discovery...");

	client->stopListeningForAdvertisements();
	delete client;
	client = NULL;
	state=NetworkStateReady;
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
	GameState::instance()->setPlayerName(id, GuiManager::instance()->GetName());
}
