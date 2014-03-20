#include <SDL/SDL_net.h>
#include <SDL/SDL_thread.h>
#include "common.h"

enum GameServerStatus {
  GameServerReady,
  GameServerRunning,
  GameServerStopped
};

class GameServer {
public:

  // initializes SDL_net
  GameServer(int port);

  // Starts a UDP server in a bg thread
  void start();

  // Stops the bg thread (eventually)
  void stop();

  // the state of the server
  GameServerStatus state;

  // the UDP port
  int port;

  // the current packets in the buffer
  std::deque<UDPpacket*> packetBuffer;

  // the packets waiting to be sent
  std::deque<UDPpacket*> sendBuffer;

  // called on every frame to check the buffer!
  void update();

  // sends game state to every client
  void sendHeartbeat();

private:

  // the bg thread
  SDL_Thread *_serverThread;

  // list of connected clients (addresses)
  std::vector<IPaddress> _clients;

  // logic of packet processing
  void processPacket(UDPpacket *packet);
};
