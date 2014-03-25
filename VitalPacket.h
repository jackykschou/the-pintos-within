#ifndef __VitalPacket_h_
#define __VitalPacket_h_

#define TAKEDAMAGE 1
#define ENEMY_RESPAWN 2
#define ENEMY_DIE 4
#define PINTO_TAKEN 8
#define PINTO_RESPAWN 16
#define WEAPON_CHANGE 32

#include "common.h"

struct VitalInfo
{
	char type;

	uint32_t flags;

	uint32_t damage;
	uint32_t weapon_index;

  float pintoPosX;
  float pintoPosY;
  float pintoPosZ;

  float playerPosX;
  float playerPosY;
  float playerPosZ;
};

class VitalPacket
{
public:
  VitalInfo info;

  VitalPacket();

  void setDamage(int);
  bool hasDamage();
  int getDamage();

  void setChangeWeapon(int);
  bool hasChangeWeapon();
  int getWeaponIndex();

  void setEnemyDie();
  bool hasEnemyDie();

  void setEnemyRespawn(float, float, float);
  bool hasEnemyRespawn();
  Ogre::Vector3 getEnemyRespawnPos();

  void setPintoTaken();
  bool hasPintoTaken();

  void setPintoRespawn(float, float, float);
  bool hasPintoRespawn();
  Ogre::Vector3 getPintoRespawnPos();
};

#endif
