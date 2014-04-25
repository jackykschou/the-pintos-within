#include "VitalPacket.h"

#include "NetworkManager.h"
#include "PlayerCharacter.h"
#include "PlayerSpawner.h"
#include "WeaponSpawner.h"

void VitalPacket::setDamage(int damage, uint32_t player_id)
{
	PlayerDamageInfo info;
	info.type = TAKEDAMAGE;
	info.player_id = NetworkManager::instance()->player_id;
	info.damage = damage;
	info.damaged_player_id = player_id;
	NetworkManager::instance()->send(&info, sizeof(PlayerDamageInfo), true);
}

void VitalPacket::receiveDamage(PlayerDamageInfo* info_p)
{
	if (info_p->player_id == NetworkManager::instance()->player_id)
		return;

	if(info_p->damaged_player_id == NetworkManager::instance()->player_id &&
			GameState::instance()->players[info_p->damaged_player_id] &&
			!GameState::instance()->players[info_p->damaged_player_id]->is_dead)
	{
		GameState::instance()->player->health -= info_p->damage;
		AudioManager::instance()->playBlootSplat(Ogre::Vector3(GameState::instance()->player->transform->posX,
												GameState::instance()->player->transform->posY,
												GameState::instance()->player->transform->posZ));
	}
}

void VitalPacket::setPlayerRespawn(float posX, float posY, float posZ, uint32_t player_id)
{
	PlayerRespawnInfo info;
	info.type = PLAYER_RESPAWN;
	info.player_id = NetworkManager::instance()->player_id;
	info.player_respawn_id = player_id;
	info.playerX = posX;
	info.playerY = posY;
	info.playerZ = posZ;
	NetworkManager::instance()->send(&info, sizeof(PlayerRespawnInfo), true);
}

void VitalPacket::receivePlayerRespawn(PlayerRespawnInfo* info_p)
{
	if (info_p->player_id == NetworkManager::instance()->player_id)
		return;

	if(GameState::instance()->players[info_p->player_respawn_id] != NULL)
	{
		((GameObject*)(GameState::instance()->players[info_p->player_respawn_id]))->scene->removeGameObject((GameObject*)GameState::instance()->players[info_p->player_respawn_id]);
		delete GameState::instance()->players[info_p->player_respawn_id];
		GameState::instance()->players[info_p->player_respawn_id] = NULL;
	}

	GameState::instance()->spawner->spawnPlayer(info_p->playerX, info_p->playerY, info_p->playerZ, info_p->player_respawn_id);
}

void VitalPacket::setPlayerDie()
{
	PlayerDieInfo info;
	info.type = PLAYER_DIE;
	info.player_id = NetworkManager::instance()->player_id;
	NetworkManager::instance()->send(&info, sizeof(PlayerDieInfo), true);
}

void VitalPacket::receivePlayerDie(PlayerDieInfo* info_p)
{
	if (info_p->player_id == NetworkManager::instance()->player_id)
		return;
	
	if(GameState::instance()->players[info_p->player_id] != NULL)
	{
		((GameObject*)(GameState::instance()->players[info_p->player_id]))->scene->removeGameObject((GameObject*)GameState::instance()->players[info_p->player_id]);
		delete GameState::instance()->players[info_p->player_id];
		GameState::instance()->players[info_p->player_id] = NULL;
	}

	if(NetworkManager::instance()->isServer())
	{
		Ogre::Vector3 pos = GameState::instance()->spawner->spawnPlayer(info_p->player_id);
		NetworkManager::instance()->vital->setPlayerRespawn(pos.x, pos.y, pos.z, info_p->player_id);
	}
}

void VitalPacket::setChangeWeapon(uint32_t index)
{
	ChangeWeaponInfo info;
	info.type = WEAPON_CHANGE;
	info.player_id = NetworkManager::instance()->player_id;
	info.weapon_index = index;
	NetworkManager::instance()->send(&info, sizeof(ChangeWeaponInfo), true);
}

void VitalPacket::receiveChangeWeapon(ChangeWeaponInfo* info_p)
{
	if (info_p->player_id == NetworkManager::instance()->player_id)
		return;

	if(GameState::instance()->players[info_p->player_id] != NULL)
		GameState::instance()->players[info_p->player_id]->changeWeapon(info_p->weapon_index);
}

void VitalPacket::setSpawnWeapon(uint32_t id, float x, float y, float z)
{
	WeaponSpawnInfo info;
	info.type = WEAPON_SPAWN;
	info.player_id = NetworkManager::instance()->player_id;
	info.weapon_id = id;
	info.spawnX = x;
 	info.spawnY = y;
  	info.spawnZ = z;
	NetworkManager::instance()->send(&info, sizeof(WeaponSpawnInfo), true);
}

void VitalPacket::receiveSpawnWeapon(WeaponSpawnInfo* info)
{
	if (info->player_id == NetworkManager::instance()->player_id)
		return;
	
	GameState::instance()->weapon_spawner->spawnWeapon(info->spawnX, info->spawnY, info->spawnZ, info->weapon_id);
}

void VitalPacket::setPlayerFireSound()
{
	PlayFireSoundInfo info;
	info.type = PLAY_FIRE_SOUND;
	info.player_id = NetworkManager::instance()->player_id;

	NetworkManager::instance()->send(&info, sizeof(PlayFireSoundInfo), true);
}

void VitalPacket::receivePlayFireSound(PlayFireSoundInfo* info_p)
{
	if (info_p->player_id == NetworkManager::instance()->player_id)
		return;

	if(GameState::instance()->players[info_p->player_id] != NULL)
	{
		AudioManager::instance()->playWeaponFire(Ogre::Vector3(GameState::instance()->players[info_p->player_id]->transform->posX, 
																GameState::instance()->players[info_p->player_id]->transform->posY, 
																GameState::instance()->players[info_p->player_id]->transform->posZ),
												GameState::instance()->players[info_p->player_id]->current_weapon->weapon_id);
	}
}

void VitalPacket::setChangePinto()
{
	ChangePintoInfo info;
	info.type = CHANGE_PINTO;
	info.player_id = NetworkManager::instance()->player_id;

	NetworkManager::instance()->send(&info, sizeof(ChangePintoInfo), true);
}

void VitalPacket::receiveChangePinto(ChangePintoInfo* info_p)
{
	if (info_p->player_id == NetworkManager::instance()->player_id)
		return;

	if(GameState::instance()->players[info_p->player_id] != NULL)
	{
		GameState::instance()->players[info_p->player_id]->changeToPinto();
	}

}

