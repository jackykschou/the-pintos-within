#include "VitalPacket.h"

#include "NetworkManager.h"
#include "PlayerCharacter.h"
#include "PlayerSpawner.h"

VitalPacket::VitalPacket()
{
	memset(&info, 0, sizeof(VitalInfo));
	info.type = VITALPACK;
}

void VitalPacket::clear()
{
	memset(&info, 0, sizeof(VitalInfo));
	info.type = VITALPACK;
	info.player_id = NetworkManager::instance()->player_id;
}

void VitalPacket::setDamage(int damage, uint32_t player_id)
{
	info.flags |= TAKEDAMAGE;
	info.damage += damage;
	info.damaged_player_id = player_id;
}

bool VitalPacket::hasDamage(VitalInfo* info_p)
{
	return ((info_p->flags & TAKEDAMAGE) == TAKEDAMAGE);
}

int VitalPacket::getDamage(VitalInfo* info_p)
{
	return info_p->damage;
}

void VitalPacket::setChangeWeapon(int index)
{
	info.flags |= WEAPON_CHANGE;
	info.weapon_index = index;
}

bool VitalPacket::hasChangeWeapon(VitalInfo* info_p)
{
	return ((info_p->flags & WEAPON_CHANGE) == WEAPON_CHANGE);
}

int VitalPacket::getWeaponIndex(VitalInfo* info_p)
{
	return info_p->weapon_index;
}

void VitalPacket::setPlayerDie()
{
	info.flags |= PLAYER_DIE;
}

bool VitalPacket::hasPlayerDie(VitalInfo* info_p)
{
	return ((info_p->flags & PLAYER_DIE) == PLAYER_DIE);
}

void VitalPacket::setPlayerRespawn(float posX, float posY, float posZ, uint32_t player_id)
{
	info.flags |= PLAYER_RESPAWN;
	info.player_respawn_id = player_id;
	info.playerX = posX;
	info.playerY = posY;
	info.playerZ = posZ;

}
bool VitalPacket::hasPlayerRespawn(VitalInfo* info_p)
{
	return ((info_p->flags & PLAYER_RESPAWN) == PLAYER_RESPAWN);
}

Ogre::Vector3 VitalPacket::getPlayerRespawnPos(VitalInfo* info_p)
{
	return Ogre::Vector3(info_p->playerX, info_p->playerY, info_p->playerZ);
}

void VitalPacket::setPintoTaken()
{
	info.flags |= PINTO_TAKEN;
}
bool VitalPacket::hasPintoTaken(VitalInfo* info_p)
{
	return ((info_p->flags & PINTO_TAKEN) == PINTO_TAKEN);
}

void VitalPacket::setPintoRespawn(float posX, float posY, float posZ, uint32_t player_respawn_id_p)
{
	info.flags |= PINTO_RESPAWN;
	info.pintoPosX = posX;
	info.pintoPosY = posY;
	info.pintoPosZ = posZ;
	info.player_respawn_id = player_respawn_id_p;
}
bool VitalPacket::hasPintoRespawn(VitalInfo* info_p)
{
	return ((info_p->flags & PINTO_RESPAWN) == PINTO_RESPAWN);
}
Ogre::Vector3 VitalPacket::getPintoRespawnPos(VitalInfo* info_p)
{
	return Ogre::Vector3(info_p->pintoPosX, info_p->pintoPosY, info_p->pintoPosZ);
}

void VitalPacket::updatePacket(VitalInfo* info_p)
{
	if(hasDamage(info_p))
	{
		if(GameState::instance()->players[info_p->damaged_player_id] &&
			!GameState::instance()->players[info_p->damaged_player_id]->is_dead)
		{
			GameState::instance()->players[info_p->damaged_player_id]->health -= info_p->damage;
		}
	}

	if(hasPlayerDie(info_p) && GameState::instance()->players[info_p->player_id])
	{
		((GameObject*)(GameState::instance()->players[info_p->player_id]))->scene->removeGameObject((GameObject*)GameState::instance()->players[info_p->player_id]);
		GameState::instance()->players[info_p->player_id] = NULL;

		if(NetworkManager::instance()->isServer())
		{
			Ogre::Vector3 pos = GameState::instance()->spawner->spawnPlayer(info_p->player_respawn_id);
			NetworkManager::instance()->vital->setPlayerRespawn(pos.x, pos.y, pos.z, info_p->player_respawn_id);
			NetworkManager::instance()->sendVital();
		}

	}

	if(hasChangeWeapon(info_p) && GameState::instance()->players[info_p->player_id])
	{
		 GameState::instance()->players[info_p->player_id]->changeWeapon(info_p->weapon_index);
	}

	if(hasPlayerRespawn(info_p))
	{
		GameState::instance()->spawner->spawnPlayer(info_p->playerX, info_p->playerY, info_p->playerZ, info_p->player_respawn_id);
	}
}
