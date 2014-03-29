#ifndef __GameClient_h_
#define __GameClient_h_

#include <SDL/SDL_net.h>
#include "GUIManager.h"
#include "AckBuffer.h"

enum GameClientStatus {
  GameClientReady,
  GameClientDiscovering,
  GameClientConnecting,
  GameClientRunning,
  GameClientStopped
};

class GameClient {

public:

  // initializes SDL_net
  GameClient(char* host, int port);
  ~GameClient();

  // open a socket and try to connect to UDP port on the server
  int connect();

  // let a client temporarily listen on a socket for advertisement
  void startListeningForAdvertisements();
  void stopListeningForAdvertisements();

  // the state of the client connection
  GameClientStatus state;

  // called once per game loop
  void update();

  // sends data to the host
  void sendData(void* data, int len, bool ack, AckId id=0, bool isResponse=false);

private:

  // the raw string of the host name we are connecting to
  char* _host;

  // the port to connect to
  int   _port;

  // the resolved IPaddress of the server we are connecting to
  IPaddress _srvadd;

  // the UDP socket we will be recv and sending through
  UDPsocket _socket;

  // the UDP socket we will receive broadcasts from
  UDPsocket _discoverySocket;

  // a temporarily allocated packet for sending on the wire
  UDPpacket* _tmpSendPacket;

  // a temporarily allocated packet for consuming the wire
  UDPpacket* _tmpRecvPacket;

  // a buffer for "remembering" sent packets that are awaiting an ACK response
  AckBuffer* _ackBuffer;

  // sends a JOIN request
  int joinGame();

  // nom nom all the packets off the wire
  int consumePackets();

  // logic of packet processing
  void processPacket(UDPpacket* packet);

  // resend any ACK-requiring packets that we did not hear back about
  void resendExpiredAcks();

  void consumeDiscoveryPackets();
};

#endif
