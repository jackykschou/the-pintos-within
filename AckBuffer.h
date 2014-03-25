#ifndef __Ack_Buffer_h_
#define __Ack_Buffer_h_

#include "common.h"
#include <SDL/SDL_net.h>

// A simple incrementing id for uniquely identifying ACK packets
typedef uint32_t AckId;

// A single ACK needs to know the remote address/port and the contents of the packet
class Ack {
 public:

  IPaddress address;
  AckId id;
  void *packetData;
  int packetLen;

  Ack(IPaddress addr, AckId id, void *packetData, int packetLen);
  ~Ack();
};

//
// An AckBuffer instance is helpful for keeping track of network ACKs
// that are needed for non-ephemeral UDP requests (essentially emulating
// reliable delivery of packets over UDP)
//
class AckBuffer {

 public:

  AckBuffer();

  // called by the sender when an ACK is required by a certain packet
  AckId beginAck(UDPpacket *p);

  // called when an ACK is received to remove the Ack from the buffer
  void endAck(AckId id);

 private:
  AckId currentId;
  std::map<AckId, Ack*> buffer;

};

#endif
