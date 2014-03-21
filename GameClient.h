#ifndef __GameClient_h_
#define __GameClient_h_

#include <SDL/SDL_net.h>

enum GameClientStatus {
  GameClientReady,
  GameClientConnecting,
  GameClientRunning,
  GameClientStopped
};

class GameClient {

public:

  // initializes SDL_net
  GameClient(char* host, int port);

  // open a socket and try to connect to UDP port on the server
  int connect();

  // the state of the client connection
  GameClientStatus state;

  // called once per game loop
  void update();

private:

  // the raw string of the host name we are connecting to
  char* _host;

  // the port to connect to
  int   _port;

  // the resolved IPaddress of the server we are connecting to
  IPaddress _srvadd;

  // the UDP socket we will be recv and sending through
  UDPsocket _socket;

  // sends a JOIN request
  int joinGame();

  // nom nom all the packets off the wire
  int consumePackets();

  // logic of packet processing
  void processPacket(UDPpacket* packet);

  // called when the server acknowledges that a client has joined
  void handleJoinAckPacket(UDPpacket *packet);

  // a temporarily allocated packet for sending on the wire
  UDPpacket *_tmpSendPacket;

  // a temporarily allocated packet for consuming the wire
  UDPpacket *_tmpRecvPacket;
};

#endif
