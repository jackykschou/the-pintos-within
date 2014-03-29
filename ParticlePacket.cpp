#include "ParticlePacket.h"

#include "ParticleManager.h"

ParticlePacket::ParticlePacket()
{
	memset(&info, 0, sizeof(ParticleInfo));
	info.type = PARTICLEPACK;
}

void ParticlePacket::clear()
{
	memset(&info, 0, sizeof(ParticleInfo));
	info.type = PARTICLEPACK;
	info.player_id = NetworkManager::instance()->player_id;
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
bool ParticlePacket::hasBlood(ParticleInfo* info_p)
{
	return ((info_p->flags & BLOOD) == BLOOD);
}

void ParticlePacket::initBlood(ParticleInfo* info_p)
{
	for(int i = 0; i < info_p->blood_len;++i)
	{
		ParticleManager::instance()->EmitBloodSpurt(Ogre::Vector3(info_p->blood_posXs[i], info_p->blood_posYs[i], info_p->blood_posZs[i]), 
												Ogre::Vector3(info_p->blood_dirXs[i], info_p->blood_dirYs[i], info_p->blood_dirZs[i]));
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

bool ParticlePacket::hasDust(ParticleInfo* info_p)
{
	return ((info_p->flags & DUST) == DUST);
}

void ParticlePacket::initDust(ParticleInfo* info_p)
{
	for(int i = 0; i < info_p->dust_len;++i)
	{
		ParticleManager::instance()->EmitDust(Ogre::Vector3(info_p->dust_posXs[i], info_p->dust_posYs[i], info_p->dust_posZs[i]), 
												Ogre::Vector3(info_p->dust_dirXs[i], info_p->dust_dirYs[i], info_p->dust_dirZs[i]));
	}
}

void ParticlePacket::setRocketExplosion(float posX, float posY, float posZ)
{
	info.flags |= ROCKET_EXPLODE;

	info.rocketExplode_posX = posX;
	info.rocketExplode_posY = posY;
	info.rocketExplode_posZ = posZ;
}

bool ParticlePacket::hasRocketExplosion(ParticleInfo* info_p)
{
	return ((info_p->flags & ROCKET_EXPLODE) == ROCKET_EXPLODE);
}

void ParticlePacket::initRocketExplosion(ParticleInfo* info_p)
{
	// ParticleManager::instance()->EmitRocketExplosion(Ogre::Vector3(info_p->rocketExplode_posX, info_p->rocketExplode_posY, info_p->rocketExplode_posZ));
}

void ParticlePacket::setPintoExplosion(float posX, float posY, float posZ)
{
	info.flags |= PINTO_EXPLODE;

	info.pintoExplode_posX = posX;
	info.pintoExplode_posY = posY;
	info.pintoExplode_posZ = posZ;
}

bool ParticlePacket::hasPintoExplosion(ParticleInfo* info_p)
{
	return ((info_p->flags & PINTO_EXPLODE) == PINTO_EXPLODE);
}

void ParticlePacket::initPintoExplosion(ParticleInfo* info_p)
{
	// ParticleManager::instance()->EmitPintoExplosion(Ogre::Vector3(info_p->pintoExplode_posX, info_p->pintoExplode_posY, info_p->pintoExplode_posZ));
}

void ParticlePacket::updateParticles(ParticleInfo* info)
{
	if(hasDust(info))
		initDust(info);

	if(hasBlood(info))
		initBlood(info);

	if(hasRocketExplosion(info))
		initRocketExplosion(info);

	if(hasPintoExplosion(info))
		initPintoExplosion(info);
}
