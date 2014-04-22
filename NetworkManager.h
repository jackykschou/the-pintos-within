#ifndef __NetworkManager_h_
#define __NetworkManager_h_

#include <SDL/SDL_net.h>
#include "Singleton.h"
#include "GameServer.h"
#include "GameClient.h"
#include "GameState.h"

#include "HeartbeatPacket.h"
#include "VitalPacket.h"
#include "ParticlePacket.h"

#define DISCOVERY_PORT 9999
#define GAME_PORT 5555
#define HEARTBEATS_PER_SEC 10
#define HEARTBEAT_MAX_DELAY 1000.0f/HEARTBEATS_PER_SEC
#define DISCOVERY_SIGNATURE "JOEV"

struct PlayerIdInfo
{
	char type;
	uint32_t player_id;
};

struct ServerAdvertisement
{
	char magic[5];
	char name[256];
	char description[256];
};

struct ChatPacket
{
	char type;
	char playerId;
	char message[256];
};

enum NetworkManagerState 
{
	NetworkStateReady,
	NetworkStateClient,
	NetworkStateServer
};

class NetworkManager : public Singleton<NetworkManager> 
{
  public:

  	HeartbeatPacket* heartbeat;
	VitalPacket*     vital;
	ParticlePacket*  particle;

	uint32_t player_id;
	uint32_t num_player;

  	NetworkManager();
  	~NetworkManager();

	NetworkManagerState state;

	void startServer();
	void startClient(const char* host);
	void startClientDiscovery();
	void update();

	bool isActive();
	bool isServer();
	bool isClient();

	void sendVital();
	void sendParticle();


	void receiveChat(ChatPacket* packet);
	void receiveHeartbeat(HeartBeatInfo* info);
	void receiveVital(VitalInfo* info);
	void receiveParticle(ParticleInfo* info);

	void changeId(uint32_t);

	// sends game state to every client
	void broadcastHeartbeat();

	GameServer* server;
	GameClient* client;

  private:

	// the timestamp on the last heartbeat
	boost::posix_time::ptime* _lastHeartbeat;
	void send(void* data, int size, bool ack);
};

#endif
