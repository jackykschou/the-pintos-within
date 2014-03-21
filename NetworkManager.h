#ifndef __NetworkManager_h_
#define __NetworkManager_h_

#include <SDL/SDL_net.h>
#include "Singleton.h"
#include "GameServer.h"
#include "GameClient.h"
#include "GameState.h"
#include "GUIManager.h"

enum NetworkManagerState {
	NetworkStateReady,
	NetworkStateClient,
	NetworkStateServer
};

class NetworkManager : public Singleton<NetworkManager> {
  public:
	NetworkManagerState state;

	void startServer();
	void startClient(char* host);
	void update();

	bool isActive();
	bool isServer();
	bool isClient();

  	GameServer* server;
  	GameClient* client;
};

#endif
