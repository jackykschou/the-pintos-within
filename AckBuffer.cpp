#include "AckBuffer.h"

namespace pt = boost::posix_time;

Ack::Ack(IPaddress addr, AckId id, void* packetData, int packetLen) {
  this->address    = addr;
  this->id         = id;
  this->packetLen  = packetLen-MEMALIGNED_SIZE(AckHeader);
  this->packetData = malloc(packetLen);
  this->sentAt     = NULL;
  memcpy(this->packetData, packetData+MEMALIGNED_SIZE(AckHeader), packetLen);
  reset();
}

Ack::~Ack() {
  free(this->packetData);
  delete this->sentAt;
}

bool Ack::isExpired() {
  pt::ptime now          = pt::microsec_clock::local_time();
  pt::time_duration diff = now - *sentAt;
  return diff.total_milliseconds() > ACK_EXPIRATION_MS;
}

void Ack::reset() {
  pt::ptime now = pt::microsec_clock::local_time();
  if (!this->sentAt) {
    delete this->sentAt;
    this->sentAt = NULL;
  }
  this->sentAt = (pt::ptime*)malloc(sizeof(pt::ptime));
  memcpy(sentAt, &now, sizeof(pt::ptime));
}

AckBuffer::AckBuffer() {
  this->currentId = 1;
}

bool AckBuffer::hasAckId(AckId id) {
  std::map<AckId, Ack*>::iterator it = buffer.find(id);
  return it != buffer.end();
}

AckId AckBuffer::injectAck(UDPpacket* packet, IPaddress addr) {
  Ack *a = new Ack(addr, ++currentId, packet->data, packet->len);
  buffer[a->id] = a;
  return a->id;
}

void AckBuffer::forgetAck(AckId id) {
  if (hasAckId(id)) {
    delete buffer[id];
    buffer.erase(id);
  }
}
