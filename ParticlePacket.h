#ifndef __ParticlePacket_h_
#define __ParticlePacket_h_

#define BLOODARRAYLENGTH 8
#define DUSTARRAYLENGTH 8

#define BLOOD 1
#define DUST 2
#define ROCKET_EXPLODE 8
#define PINTO_EXPLODE 16
#define JETPACK 32

#include "common.h"

struct ParticleInfo
{
	char type;

	int flags;

	uint_32 blood_len;

	float blood_posXs[BLOODARRAYLENGTH];
	float blood_posYs[BLOODARRAYLENGTH];
	float blood_posZs[BLOODARRAYLENGTH];

	float blood_normalXs[BLOODARRAYLENGTH];
	float blood_normalYs[BLOODARRAYLENGTH];
	float blood_normalZs[BLOODARRAYLENGTH];

	uint_32 dust_len;

	float dust_posXs[DUSTARRAYLENGTH];
	float dust_posYs[DUSTARRAYLENGTH];
	float dust_posZs[DUSTARRAYLENGTH];

	float dust_normalXs[DUSTARRAYLENGTH];
	float dust_normalYs[DUSTARRAYLENGTH];
	float dust_normalZs[DUSTARRAYLENGTH];

	float rocketExplode_posXs;
	float rocketExplode_posYs;
	float rocketExplode_posZs;

	float pintoExplode_posXs;
	float pintoExplode_posYs;
	float pintoExplode_posZs;

	float jet_posXs;
	float jet_posYs;
	float jet_posZs;

	float jet_normalXs;
	float jet_normalYs;
	float jet_normalZs;	

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

  void setJetPack(float, float, float, float, float, float);
  bool hasJetPack();
  void initJetPack();
};

#endif