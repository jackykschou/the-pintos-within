#include "ParticlePacket.h"

ParticlePacket::ParticlePacket()
{
	info.type = PARTICLEPACK;
	info.flags = 0;

	info.blood_len = 0;
	info.dust_len = 0;
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

}

void ParticlePacket::setRocketExplosion(float posX, float posY, float posZ)
{
	info.flags |= ROCKET_EXPLODE;

	info.rocketExplode_posXs = posX;
	info.rocketExplode_posYs = posY;
	info.rocketExplode_posZs = posZ;
}

bool ParticlePacket::hasRocketExplosion()
{
	return ((info.flags & ROCKET_EXPLODE) == ROCKET_EXPLODE);
}

void ParticlePacket::initRocketExplosion()
{

}

void ParticlePacket::setPintoExplosion(float posX, float posY, float posZ)
{
	info.flags |= PINTO_EXPLODE;

	info.pintoExplode_posXs = posX;
	info.pintoExplode_posYs = posY;
	info.pintoExplode_posZs = posZ;
}

bool ParticlePacket::hasPintoExplosion()
{
	return ((info.flags & PINTO_EXPLODE) == PINTO_EXPLODE);
}

void ParticlePacket::initPintoExplosion()
{

}
