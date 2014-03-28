#ifndef __GameServer_h_
#define __GameServer_h_

#include <SDL/SDL_net.h>
#include "GUIManager.h"
#include "common.h"
#include "AckBuffer.h"

#define HEARTBEATS_PER_SEC 10

#define HEARTBEAT_MAX_DELAY 1000.0f/HEARTBEATS_PER_SEC

enum GameServerStatus {
  GameServerReady,
  GameServerRunning,
  GameServerStopped
};

class GameServer {

public:

  GameServer(int port);
  ~GameServer();

  // Starts a UDP server in a bg thread
  int start();

  // Stops the bg thread (eventually)
  void stop();

  // the state of the server
  GameServerStatus state;

  // called once per game loop
  void update();

  // sends GAME START event to every client
  void broadcastGameStart();

  // broadcasts a single chunk of data to a bunch of clients
  // this method can be used for binary or cstring (NULL terminated) buffer
  void broadcastData(void* data, int len, bool ack);

  // sends a chunk of data to a single client
  void sendDataToClient(void* data, int len, IPaddress* ip, bool ack, AckId id=0, bool isResponse=false);


private:

  // the timestamp on the last heartbeat
  boost::posix_time::ptime* _lastHeartbeat;

  // the socket that is bound
  UDPsocket _socket;

  // the UDP port
  int _port;

  // list of connected clients (addresses)
  std::vector<IPaddress> _clients;

  // nom nom all the packets off the wire
  void consumePackets();

  // logic of single packet processing
  void processPacket(UDPpacket* packet);

  // handles saving client and sending ACK
  void handleJoinPacket(UDPpacket* packet);

  // sends a single packet back to a single client
  void sendPacketToClient(UDPpacket* packet, IPaddress* ip);

  // resend any ACK-requiring packets that we did not hear back about
  void resendExpiredAcks();

  // this puts an AckHeader array above the data in the packet, and adds
  // the request to the ack buffer if necessary.
  void putDataIntoPacket(UDPpacket *p, void *data, int len,
                         IPaddress* a, bool ack, AckId id=-1, bool isResponse=false);

  // broadcasts a single packet to all connected clients
  void broadcastPacket(UDPpacket* packet, bool ack);

  // broadcasts a single cstring (data->"\x00") to a bunch of clients
  void broadcastString(const char* data, bool ack);

  // sends game state to every client
  void broadcastHeartbeat();

  // a temporarily allocated packet for sending on the wire
  UDPpacket* _tmpSendPacket;

  // a temporarily allocated packet for consuming the wire
  UDPpacket* _tmpRecvPacket;

  // a buffer for "remembering" sent packets that are awaiting an ACK response
  AckBuffer* _ackBuffer;
};

#endif
