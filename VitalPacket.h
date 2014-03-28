#ifndef __VitalPacket_h_
#define __VitalPacket_h_

#define TAKEDAMAGE 1
#define CLIENTPLAYER_RESPAWN 2
#define CLIENTPLAYER_DIE 4
#define PINTO_TAKEN 8
#define PINTO_RESPAWN 16
#define WEAPON_CHANGE 32
#define GAME_START 64
#define PLAYER_DIE 128
#define PLAYER_REPAWN 256

#include "common.h"

struct VitalInfo
{
	char type;

  uint32_t player_id;

	uint32_t flags;

	uint32_t damage;
	uint32_t weapon_index;

  float pintoPosX;
  float pintoPosY;
  float pintoPosZ;

  float playerX;
  float playerY;
  float playerZ;
};

class VitalPacket
{
public:
  VitalInfo info;

  VitalPacket();

  void clear();

  void setDamage(int);
  bool hasDamage();
  int getDamage();

  void setChangeWeapon(int);
  bool hasChangeWeapon();
  int getWeaponIndex();

  void setPlayerDie();
  bool hasPlayerDie();

  void setClientPlayerRespawn(float, float, float);
  bool hasClientPlayerRespawn();
  Ogre::Vector3 getClientPlayerRespawnPos();

  void setHostPlayerRespawn(float, float, float);
  bool hasHostPlayerRespawn();
  Ogre::Vector3 getHostPlayerRespawnPos();

  void setPintoTaken();
  bool hasPintoTaken();

  void setPintoRespawn(float, float, float);
  bool hasPintoRespawn();
  Ogre::Vector3 getPintoRespawnPos();
};

#endif
