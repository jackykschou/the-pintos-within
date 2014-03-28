#ifndef __ParticlePacket_h_
#define __ParticlePacket_h_

#define BLOODARRAYLENGTH 8
#define DUSTARRAYLENGTH 8

#define BLOOD 1
#define DUST 2
#define ROCKET_EXPLODE 8
#define PINTO_EXPLODE 16

#include "common.h"

struct ParticleInfo
{
	char type;

	uint32_t player_id;

	uint32_t flags;

	uint32_t blood_len;

	float blood_posXs[BLOODARRAYLENGTH];
	float blood_posYs[BLOODARRAYLENGTH];
	float blood_posZs[BLOODARRAYLENGTH];

	float blood_dirXs[BLOODARRAYLENGTH];
	float blood_dirYs[BLOODARRAYLENGTH];
	float blood_dirZs[BLOODARRAYLENGTH];

	uint32_t dust_len;

	float dust_posXs[DUSTARRAYLENGTH];
	float dust_posYs[DUSTARRAYLENGTH];
	float dust_posZs[DUSTARRAYLENGTH];

	float dust_dirXs[DUSTARRAYLENGTH];
	float dust_dirYs[DUSTARRAYLENGTH];
	float dust_dirZs[DUSTARRAYLENGTH];

	float rocketExplode_posX;
	float rocketExplode_posY;
	float rocketExplode_posZ;

	float pintoExplode_posX;
	float pintoExplode_posY;
	float pintoExplode_posZ;
};

class ParticlePacket
{
public:
  ParticleInfo info;

  ParticlePacket();

  void clear();

  void setBlood(float, float, float, float, float, float);
  bool hasBlood(ParticleInfo*);
  void initBlood(ParticleInfo*);

  void setDust(float, float, float, float, float, float);
  bool hasDust(ParticleInfo*);
  void initDust(ParticleInfo*);

  void setRocketExplosion(float, float, float);
  bool hasRocketExplosion(ParticleInfo*);
  void initRocketExplosion(ParticleInfo*);

  void setPintoExplosion(float, float, float);
  bool hasPintoExplosion(ParticleInfo*);
  void initPintoExplosion(ParticleInfo*);

  void updateParticles(ParticleInfo*);
};

#endif