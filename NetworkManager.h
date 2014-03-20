#include <SDL/SDL_net.h>
#include "Singleton.h"
#include "GameServer.h"

enum NetworkManagerState {
	NetworkReady,
	NetworkClient,
	NetworkHost
};

class NetworkManager : public Singleton<NetworkManager> {
	NetworkManagerState state;

	void startServer();
	void startClient(char *host);

	bool isActive();
	bool isServer();
	bool isClient();

  private:
  	GameServer *_server;
  	GameClient *_client;
};
