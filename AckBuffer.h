#ifndef __Ack_Buffer_h_
#define __Ack_Buffer_h_

#include "common.h"
#include <SDL/SDL_net.h>

// A simple incrementing id for uniquely identifying ACK packets
typedef uint32_t AckId;

#define ACK_EXPIRATION_MS 40

// The AckPacket struct is useful for serializing the ACK
// for pushing over the wire.
// We shove this on top of every request that goes out,
// whether or not an ACK is required by the request.
struct AckPacket {
  // alignment will no doubt screw us here and add
  // 3-4 extra bytes. I could pack this into the lower
  // bit of AckId to save some bytes, but for semantics
  // I won't do this hack.
  bool isResponse;
  bool ackRequired;
  // the local id of the ACK.
  AckId id;
};

// A single ACK needs to know the remote address/port and the contents of the packet
class Ack {
 public:

  IPaddress address;
  AckId id;

  // this is a pointer to packet contents without the AckPacket
  void *packetData;

  // this is the length of packet data without the AckPacket
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

  // whether a specified ACK exists
  bool hasAckId(AckId) ;

  // a mapping from id -> original request
  std::map<AckId, Ack*> buffer;


 private:

  // the auto-incrementing id of the ACK
  AckId currentId;

};

#endif
