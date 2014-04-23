#ifndef __ParticlePacket_h_
#define __ParticlePacket_h_

#include "common.h"

struct DustInfo
{
	char type;
	uint32_t player_id;

	float posX;
	float posY;
	float posZ;

	float dirX;
	float dirY;
	float dirZ;
};

struct BloodInfo
{
	char type;
	uint32_t player_id;

	float posX;
	float posY;
	float posZ;

	float dirX;
	float dirY;
	float dirZ;
};

struct BlasterExplodeInfo
{
	char type;
	uint32_t player_id;

	float posX;
	float posY;
	float posZ;

	float radius;
};

class ParticlePacket
{
public:
  void setBlood(float, float, float, float, float, float);
  void receiveBlood(BloodInfo*);

  void setDust(float, float, float, float, float, float);
  void receiveDust(DustInfo*);

  void setBlasterExplosion(float, float, float, float);
  void receiveBlasterExplosion(BlasterExplodeInfo*);
};

#endif