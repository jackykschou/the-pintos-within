#include "ParticlePacket.h"

#include "ParticleManager.h"

ParticlePacket::ParticlePacket()
{
	info.type = PARTICLEPACK;
	info.flags = 0;

	info.blood_len = 0;
	info.dust_len = 0;
}

void ParticlePacket::clear()
{
	memset(&info, 0, sizeof(ParticleInfo));
	info.type = PARTICLEPACK;
}

void ParticlePacket::setBlood(float posX, float posY, float posZ, float dirX, float dirY, float dirZ)
{
	info.flags |= BLOOD;

	if(info.blood_len < BLOODARRAYLENGTH)
	{
		info.blood_posXs[info.blood_len] = posX;
		info.blood_posYs[info.blood_len] = posY;
		info.blood_posZs[info.blood_len] = posZ;
		info.blood_dirXs[info.blood_len] = dirX;
		info.blood_dirYs[info.blood_len] = dirY;
		info.blood_dirZs[info.blood_len] = dirZ;
		++info.blood_len;
	}
}
bool ParticlePacket::hasBlood()
{
	return ((info.flags & BLOOD) == BLOOD);
}

void ParticlePacket::initBlood()
{
	for(int i = 0; i < info.blood_len;++i)
	{
		ParticleManager::instance()->EmitSparks(Ogre::Vector3(info.blood_posXs[i], info.blood_posYs[i], info.blood_posZs[i]), 
												Ogre::Vector3(info.blood_dirXs[i], info.blood_dirYs[i], info.blood_dirZs[i]));
	}
}

void ParticlePacket::setDust(float posX, float posY, float posZ, float dirX, float dirY, float dirZ)
{
	info.flags |= DUST;

	if(info.dust_len < DUSTARRAYLENGTH)
	{
		info.dust_posXs[info.dust_len] = posX;
		info.dust_posYs[info.dust_len] = posY;
		info.dust_posZs[info.dust_len] = posZ;
		info.dust_dirXs[info.dust_len] = dirX;
		info.dust_dirYs[info.dust_len] = dirY;
		info.dust_dirZs[info.dust_len] = dirZ;
		++info.dust_len;
	}
}

bool ParticlePacket::hasDust()
{
	return ((info.flags & DUST) == DUST);
}

void ParticlePacket::initDust()
{
	for(int i = 0; i < info.dust_len;++i)
	{
		ParticleManager::instance()->EmitSparks(Ogre::Vector3(info.dust_posXs[i], info.dust_posYs[i], info.dust_posZs[i]), 
												Ogre::Vector3(info.dust_dirXs[i], info.dust_dirYs[i], info.dust_dirZs[i]));
	}
}

void ParticlePacket::setRocketExplosion(float posX, float posY, float posZ)
{
	info.flags |= ROCKET_EXPLODE;

	info.rocketExplode_posX = posX;
	info.rocketExplode_posY = posY;
	info.rocketExplode_posZ = posZ;
}

bool ParticlePacket::hasRocketExplosion()
{
	return ((info.flags & ROCKET_EXPLODE) == ROCKET_EXPLODE);
}

void ParticlePacket::initRocketExplosion()
{
	// ParticleManager::instance()->EmitRocketExplosion(Ogre::Vector3(info.rocketExplode_posX, info.rocketExplode_posY, info.rocketExplode_posZ));
}

void ParticlePacket::setPintoExplosion(float posX, float posY, float posZ)
{
	info.flags |= PINTO_EXPLODE;

	info.pintoExplode_posX = posX;
	info.pintoExplode_posY = posY;
	info.pintoExplode_posZ = posZ;
}

bool ParticlePacket::hasPintoExplosion()
{
	return ((info.flags & PINTO_EXPLODE) == PINTO_EXPLODE);
}

void ParticlePacket::initPintoExplosion()
{
	// ParticleManager::instance()->EmitPintoExplosion(Ogre::Vector3(info.pintoExplode_posX, info.pintoExplode_posY, info.pintoExplode_posZ));
}

void ParticlePacket::updateParticles()
{
	initDust();
	initBlood();
	initRocketExplosion();
	initPintoExplosion();
}
