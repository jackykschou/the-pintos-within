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

	float rocketExplode_posXs;
	float rocketExplode_posYs;
	float rocketExplode_posZs;

	float pintoExplode_posXs;
	float pintoExplode_posYs;
	float pintoExplode_posZs;
};

class ParticlePacket
{
public:
  ParticleInfo info;

  ParticlePacket();

  void setBlood(float, float, float, float, float, float);
  bool hasBlood();
  void initBlood();

  void setDust(float, float, float, float, float, float);
  bool hasDust();
  void initDust();

  void setRocketExplosion(float, float, float);
  bool hasRocketExplosion();
  void initRocketExplosion();

  void setPintoExplosion(float, float, float);
  bool hasPintoExplosion();
  void initPintoExplosion();
};

#endif