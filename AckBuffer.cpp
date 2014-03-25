#include "AckBuffer.h"

Ack::Ack(IPaddress addr, AckId id, void* packetData, int packetLen) {
  this->address = addr;
  this->id = id;
  this->packetLen = packetLen;
  this->packetData = malloc(packetLen);
  memcpy(this->packetData, packetData, packetLen);
}

Ack::~Ack() {
  free(this->packetData);
}

AckBuffer::AckBuffer() {
  this->currentId = 0;
}


AckId AckBuffer::beginAck(UDPpacket* packet) {
  Ack a(packet->address, currentId++, packet->data, packet->len);
  buffer[a.id] = a;
  return a.id;
}

void AckBuffer::endAck(AckId id) {
  std::map<char,int>::iterator it = buffer.find(id);
  if (it != buffer.end()) {
    buffer.erase(id);
  }
}
