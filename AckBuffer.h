#ifndef __Ack_Buffer_h_
#define __Ack_Buffer_h_

#include "common.h"
#include <SDL/SDL_net.h>

// A simple incrementing id for uniquely identifying ACK packets
typedef uint32_t AckId;

#define ACK_EXPIRATION_MS 40

// A single ACK needs to know the remote address/port and the contents of the packet
class Ack {
 public:

  IPaddress address;
  AckId id;
  void *packetData;
  int packetLen;
  boost::posix_time::ptime *sentAt;

  Ack(IPaddress addr, AckId id, void *packetData, int packetLen);
  ~Ack();

  bool isExpired();
  void reset();
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
  AckId injectAck(UDPpacket* packet, IPaddress addr);

  // called when an ACK is received to remove the Ack from the buffer
  void forgetAck(AckId id);

  // a mapping from id -> original request
  std::map<AckId, Ack*> buffer;


 private:

  // the auto-incrementing id of the ACK
  AckId currentId;

};

#endif
