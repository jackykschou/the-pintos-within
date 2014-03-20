#include <SDL/SDL_net.h>

enum GameClientStatus {
  GameClientReady,
  GameClientConnecting,
  GameClientRunning,
  GameClientStopped
};

class GameClient {
public:
	GameClient::GameClient(char *host);
	void connect(int port);
	GameClientStatus state;

private:
	char *_host;
	IPaddress _srvadd;
	UDPsocket _socket;
	int joinGame();
	int consumePackets();
	int processPacket(UDPpacket *p);
};
