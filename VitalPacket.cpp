#include "VitalPacket.h"

VitalPacket::VitalPacket()
{
	info.type = VITALPACK;

	info.flags = 0;

	info.damage = 0;
	info.weapon_index = 0;
}

void VitalPacket::clear()
{
	memset(&info, 0, sizeof(VitalInfo));
	info.type = VITALPACK;
}

void VitalPacket::setDamage(int damage)
{
	info.flags |= TAKEDAMAGE;
	info.damage += damage;
}

bool VitalPacket::hasDamage()
{
	return ((info.flags & TAKEDAMAGE) == TAKEDAMAGE);
}

int VitalPacket::getDamage()
{
	return info.damage;
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
	return info.weapon_index;
}

void VitalPacket::setClientPlayerDie()
{
	info.flags |= CLIENTPLAYER_DIE;
}

bool VitalPacket::hasClientPlayerDie()
{
	return ((info.flags & CLIENTPLAYER_DIE) == CLIENTPLAYER_DIE);
}

void VitalPacket::setClientPlayerRespawn(float posX, float posY, float posZ)
{
	info.flags |= CLIENTPLAYER_RESPAWN;
	info.clientPlayerX = posX;
	info.clientPlayerY = posY;
	info.clientPlayerZ = posZ;

}
bool VitalPacket::hasClientPlayerRespawn()
{
	return ((info.flags & CLIENTPLAYER_RESPAWN) == CLIENTPLAYER_RESPAWN);
}

Ogre::Vector3 VitalPacket::getClientPlayerRespawnPos()
{
	return Ogre::Vector3(info.clientPlayerX, info.clientPlayerY, info.clientPlayerZ);
}

void VitalPacket::setHostPlayerDie()
{
	info.flags |= HOSTPLAYER_DIE;
}

bool VitalPacket::hasHostPlayerDie()
{
	return ((info.flags & HOSTPLAYER_DIE) == HOSTPLAYER_DIE);
}

void VitalPacket::setHostPlayerRespawn(float posX, float posY, float posZ)
{
	info.flags |= HOSTPLAYER_REPAWN;
	info.hostPlayerX = posX;
	info.hostPlayerY = posY;
	info.hostPlayerZ = posZ;

}
bool VitalPacket::hashostPlayerRespawn()
{
	return ((info.flags & HOSTPLAYER_REPAWN) == HOSTPLAYER_REPAWN);
}

Ogre::Vector3 VitalPacket::getHostPlayerRespawnPos()
{
	return Ogre::Vector3(info.hostPlayerX, info.hostPlayerY, info.hostPlayerZ);
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
	info.pintoPosX = posX;
	info.pintoPosY = posY;
	info.pintoPosZ = posZ;
}
bool VitalPacket::hasPintoRespawn()
{
	return ((info.flags & PINTO_RESPAWN) == PINTO_RESPAWN);
}
Ogre::Vector3 VitalPacket::getPintoRespawnPos()
{
	return Ogre::Vector3(info.pintoPosX, info.pintoPosY, info.pintoPosZ);
}

