#ifndef __VitalPacket_h_
#define __VitalPacket_h_

#include "common.h"

struct PlayerDamageInfo
{
  char type;
  uint32_t player_id;
  uint32_t damage;
  uint32_t damaged_player_id;
};

struct PlayerRespawnInfo
{
  char type;
  uint32_t player_id;
  uint32_t player_respawn_id;
  float playerX;
  float playerY;
  float playerZ;
};

struct PlayerDieInfo
{
  char type;
  uint32_t player_id;
};

struct ChangeWeaponInfo
{
  char type;
  uint32_t player_id;
  uint32_t weapon_index;
};

struct WeaponSpawnInfo
{
  char type;
  uint32_t player_id;
  uint32_t weapon_id;
  float spawnX;
  float spawnY;
  float spawnZ;
};

class VitalPacket
{
public:
  void setDamage(int damage, uint32_t player_id);
  void receiveDamage(PlayerDamageInfo* info);
  void setPlayerRespawn(float posX, float posY, float posZ, uint32_t player_id);
  void receivePlayerRespawn(PlayerRespawnInfo* info_p);
  void setPlayerDie();
  void receivePlayerDie(PlayerDieInfo* info_p);
  void setChangeWeapon(uint32_t index);
  void receiveChangeWeapon(ChangeWeaponInfo* info_p);
  void setSpawnWeapon(uint32_t id, float x, float y, float z);
  void receiveSpawnWeapon(WeaponSpawnInfo* info);
};

#endif
