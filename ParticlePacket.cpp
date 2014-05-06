#include "ParticlePacket.h"
#include "ParticleManager.h"

void ParticlePacket::setBlood(float posX, float posY, float posZ, float dirX, float dirY, float dirZ)
{
	BloodInfo info;
	info.type = BLOOD;
	info.player_id = NetworkManager::instance()->player_id;
	info.posX = posX;
	info.posY = posY;
	info.posZ = posZ;
	info.dirX = dirX;
	info.dirY = dirY;
	info.dirZ = dirZ;

	NetworkManager::instance()->send(&info, sizeof(BloodInfo), true);
}

void ParticlePacket::receiveBlood(BloodInfo* info_p)
{
	if (info_p->player_id == NetworkManager::instance()->player_id || !GameState::instance()->isRunning())
		return;

	ParticleManager::instance()->EmitBloodSpurt(Ogre::Vector3(info_p->posX, info_p->posY, info_p->posZ), 
											Ogre::Vector3(info_p->dirX, info_p->dirY, info_p->dirZ));
}

void ParticlePacket::setDust(float posX, float posY, float posZ, float dirX, float dirY, float dirZ)
{
	DustInfo info;
	info.type = DUST;
	info.player_id = NetworkManager::instance()->player_id;
	info.posX = posX;
	info.posY = posY;
	info.posZ = posZ;
	info.dirX = dirX;
	info.dirY = dirY;
	info.dirZ = dirZ;

	NetworkManager::instance()->send(&info, sizeof(DustInfo), true);
}

void ParticlePacket::receiveDust(DustInfo* info_p)
{
	if (info_p->player_id == NetworkManager::instance()->player_id || !GameState::instance()->isRunning())
		return;

	ParticleManager::instance()->EmitDust(Ogre::Vector3(info_p->posX, info_p->posY, info_p->posZ), 
											Ogre::Vector3(info_p->dirX, info_p->dirY, info_p->dirZ));
}

void ParticlePacket::setBlasterExplosion(float posX, float posY, float posZ, float radius)
{
	BlasterExplodeInfo info;
	info.type = BLASTER_EXPLODE;
	info.player_id = NetworkManager::instance()->player_id;
	info.posX = posX;
	info.posY = posY;
	info.posZ = posZ;
	info.radius = radius;
	
	NetworkManager::instance()->send(&info, sizeof(BlasterExplodeInfo), true);
}

void ParticlePacket::receiveBlasterExplosion(BlasterExplodeInfo* info_p)
{
	if (info_p->player_id == NetworkManager::instance()->player_id || !GameState::instance()->isRunning())
		return;
	
	ParticleManager::instance()->EmitRocketExplosion(Ogre::Vector3(info_p->posX, info_p->posY, info_p->posZ));
}
