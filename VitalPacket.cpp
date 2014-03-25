#include "VitalPacket.h"

VitalPacket::VitalPacket()
{
	info.type = VITALPACK;

	infoflags = 0;

	info.damage = 0;
	info.weapon_index = 0;
}

void VitalPacket::setDamage(int damage)
{
	info.flags |= TAKEDAMAGE;
	info.damage = damage;
}

bool VitalPacket::hasDamage()
{
	return ((info.flags & TAKEDAMAGE) == TAKEDAMAGE);
}

int VitalPacket::getDamage()
{
	return damage;
}

void VitalPacket::setChangeWeapon(int index)
{
	info.flags |= WEAPON_CHANGE;
	info.weapon_index = index;
}

bool VitalPacket::hasChangeWeapon()
{
	return ((info.flags & WEAPON_CHANGE) == WEAPON_CHANGE);
}

int VitalPacket::getWeaponIndex()
{
	return weapon_index;
}

void VitalPacket::setEnemyDie()
{
	info.flags |= ENEMY_DIE;
}

bool VitalPacket::hasEnemyDie()
{
	return ((info.flags & ENEMY_DIE) == ENEMY_DIE);
}

void VitalPacket::setEnemyRespawn(float posX, float posY, float posZ)
{
	info.flags |= ENEMY_RESPAWN;
	playerPosX = posX;
	playerPosY = posY;
	playerPosZ = posZ;

}
bool VitalPacket::hasEnemyRespawn()
{
	return ((info.flags & ENEMY_RESPAWN) == ENEMY_RESPAWN);
}

Ogre::Vector3 VitalPacket::getEnemyRespawnPos()
{
	return Ogre::Vector3(playerPosX, playerPosY, playerPosZ);
}

void VitalPacket::setPintoTaken()
{
	info.flags |= PINTO_TAKEN;
}
bool VitalPacket::hasPintoTaken()
{
	return ((info.flags & PINTO_TAKEN) == PINTO_TAKEN);
}

void VitalPacket::setPintoRespawn(float posX, float posY, float posZ)
{
	info.flags |= PINTO_RESPAWN;
	pintoPosX = posX;
	pintoPosY = posY;
	pintoPosZ = posZ;
}
bool VitalPacket::hasPintoRespawn()
{
	return ((info.flags & PINTO_RESPAWN) == PINTO_RESPAWN);
}
Ogre::Vector3 VitalPacket::getPintoRespawnPos()
{
	return Ogre::Vector3(pintoPosX, pintoPosY, pintoPosZ);
}

