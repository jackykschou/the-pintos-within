#ifndef __VitalPacket_h_
#define __VitalPacket_h_

#define TAKEDAMAGE 1
#define PLAYER_RESPAWN 2
#define PLAYER_DIE 4
#define PINTO_TAKEN 8
#define PINTO_RESPAWN 16
#define WEAPON_CHANGE 32

#include "common.h"

struct VitalInfo
{
	char type;

  uint32_t player_id;

	uint32_t flags;

	uint32_t damage;
  uint32_t damaged_player_id;

	uint32_t weapon_index;

  float pintoPosX;
  float pintoPosY;
  float pintoPosZ;

  uint32_t player_respawn_id;
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

  void setDamage(int, uint32_t);
  bool hasDamage(VitalInfo*);
  int getDamage(VitalInfo*);

  void setChangeWeapon(int);
  bool hasChangeWeapon(VitalInfo*);
  int getWeaponIndex(VitalInfo*);

  void setPlayerDie();
  bool hasPlayerDie(VitalInfo*);

  void setPlayerRespawn(float, float, float, uint32_t);
  bool hasPlayerRespawn(VitalInfo*);
  Ogre::Vector3 getPlayerRespawnPos(VitalInfo*);

  void setPintoTaken();
  bool hasPintoTaken(VitalInfo*);

  void setPintoRespawn(float, float, float, uint32_t);
  bool hasPintoRespawn(VitalInfo*);
  Ogre::Vector3 getPintoRespawnPos(VitalInfo*);

  void updatePacket(VitalInfo*);
};

#endif
