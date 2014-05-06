#ifndef __VitalPacket_h_
#define __VitalPacket_h_

#include "common.h"

struct PlayerDamageInfo
{
  char type;
  uint32_t player_id;
  uint32_t damage;
  uint32_t damage_dealer_id;
  uint32_t damaged_player_id;
};

struct PlayerRespawnInfo
{
  char type;
  uint32_t player_id;
  uint32_t version;
  uint32_t player_respawn_id;
  float playerX;
  float playerY;
  float playerZ;
  uint32_t is_pinto;
  uint32_t team_id;
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

struct ChangeHairInfo
{
  char type;
  uint32_t player_id;
  uint32_t hair_index;
};

struct WeaponSpawnInfo
{
  char type;
  uint32_t player_id;
  uint32_t weapon_id;
  float spawnX;
  float spawnY;
  float spawnZ;
  uint32_t pick_up_id;
  uint32_t pick_up_type;
};

struct PlayFireSoundInfo
{
  char type;
  uint32_t player_id;
};

struct ChangePintoInfo
{
  char type;
  uint32_t player_id;
  uint32_t to_pinto_player_id;
};

struct IncreaseScoreInfo
{
  char type;
  uint32_t receive_player_id;
  uint32_t receive_team_id;
  uint32_t amount;
};

struct TimeLeftInfo
{
  char type;
  uint32_t player_id;
  uint32_t time_left;
};

class VitalPacket
{
public:
  void setDamage(int damage, uint32_t player_id);
  void receiveDamage(PlayerDamageInfo* info);
  void setPlayerRespawn(float posX, float posY, float posZ, uint32_t player_id, uint32_t team_id, uint32_t version, bool);
  void receivePlayerRespawn(PlayerRespawnInfo* info_p);
  void setPlayerDie();
  void receivePlayerDie(PlayerDieInfo* info_p);
  void setChangeWeapon(uint32_t index);
  void receiveChangeWeapon(ChangeWeaponInfo* info_p);
  void setSpawnWeapon(uint32_t id, float x, float y, float z, int, int);
  void receiveSpawnWeapon(WeaponSpawnInfo* info);
  void setPlayerFireSound();
  void receivePlayFireSound(PlayFireSoundInfo* info_p);
  void setChangePinto(uint32_t);
  void receiveChangePinto(ChangePintoInfo* info_p);
  void setIncreaseScore(uint32_t player_id, uint32_t amount, uint32_t team_id);
  void receiveIncreaseScore(IncreaseScoreInfo* info_p);
  void setTimeLeft(uint32_t);
  void receiveTimeLeft(TimeLeftInfo*);
  void setChangeHair(uint32_t index);
  void receiveChangeHair(ChangeHairInfo* info_p);
};

#endif
