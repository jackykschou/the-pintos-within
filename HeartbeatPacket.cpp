#include "HeartbeatPacket.h"

#include "PlayerCharacter.h"
#include "PhysicsManager.h"

HeartbeatPacket::HeartbeatPacket()
{
	memset(&info, 0, sizeof(HeartBeatInfo));
	info.type = HEARTBEATPACK;

	info.playerRotW = 1;
}

void HeartbeatPacket::clear()
{
	memset(&info, 0, sizeof(HeartBeatInfo));
	info.type = HEARTBEATPACK;
	info.player_id = NetworkManager::instance()->player_id;

	info.playerRotW = 1;
}

void HeartbeatPacket::renewPlayerInfo(PlayerCharacter* player)
{
	info.flags = 0;

	info.type = HEARTBEATPACK;

	info.player_id = NetworkManager::instance()->player_id;

	Transform* tran = ((GameObject*)player)->getComponent<Transform>();

	info.playerPosX = tran->posX;
	info.playerPosY = tran->posY;
	info.playerPosZ = tran->posZ;

	info.playerRotX = tran->rotX;
	info.playerRotY = tran->rotY;
	info.playerRotZ = tran->rotZ;
	info.playerRotW = tran->rotW;

	info.flags |= player->is_dead ? DEAD : 0;
	info.flags |= player->is_shooting ? SHOOTING : 0;
	info.flags |= player->is_moving ? MOVING : 0;
	info.flags |= player->is_idle ? IDLE : 0;
	info.flags |= player->is_reloading ? RELOADING : 0;
	info.flags |= player->is_jet_packing ? JETPACKING : 0;
	info.flags |= player->is_jumping ? JUMPING : 0;

	info.velocityX = player->controller->controller->m_walkDirection.getX();
	info.velocityY = player->controller->controller->m_walkDirection.getY();
	info.velocityZ = player->controller->controller->m_walkDirection.getZ();

	info.run_animation_time 	= player->run_animation_time;
	info.shoot_animation_time 	= player->shoot_animation_time;
	info.idle_animation_time 	= player->idle_animation_time;
	info.reload_animation_time 	= player->reload_animation_time;
	info.jump_animation_time	= player->jump_animation_time;
	info.die_animation_time		= player->die_animation_time;
}

void HeartbeatPacket::updatePlayer(HeartBeatInfo* info_p, PlayerCharacter* player)
{
	Transform* tran = ((GameObject*)player)->getComponent<Transform>();

	// LOG("Time since last frame: " << GraphicsManager::instance()->getFrameEvent()->timeSinceLastFrame);
	// LOG("Transform before: " << tran->posX << " " << tran->posY << " " << tran->posZ);
	// LOG("Transform after: " << info_p->playerPosX << " " << info_p->playerPosY << " " << info_p->playerPosZ);
	// LOG("Velocity before: " << player->controller->controller->m_walkDirection.getX() << " " << 
	// 														player->controller->controller->m_walkDirection.getY() << " " << 
	// 														player->controller->controller->m_walkDirection.getZ());
	// LOG("Velocity after: " << info_p->velocityX << " " << info_p->velocityY << " " << info_p->velocityZ);

	// tran->posX = info_p->playerPosX;
	// tran->posY = info_p->playerPosY;
	// tran->posZ = info_p->playerPosZ;

	player->controller->_ghostObject->setWorldTransform(btTransform(btQuaternion(tran->rotX, tran->rotY, tran->rotZ, tran->rotW), 
									btVector3(info_p->playerPosX, info_p->playerPosY, info_p->playerPosZ)));

	tran->rotX = info_p->playerRotX;
	tran->rotY = info_p->playerRotY;
	tran->rotZ = info_p->playerRotZ;
	tran->rotW = info_p->playerRotW;

	player->controller->_ghostObject->setWorldTransform(btTransform(btQuaternion(tran->rotX, tran->rotY, tran->rotZ, tran->rotW), 
									btVector3(info_p->playerPosX, info_p->playerPosY, info_p->playerPosZ)));

	player->is_dead			= ((info_p->flags & DEAD) == DEAD) ? true : false;
	player->is_shooting		= ((info_p->flags & SHOOTING) == SHOOTING) ? true : false;
	player->is_moving		= ((info_p->flags & MOVING) == MOVING) ? true : false;
	player->is_idle			= ((info_p->flags & IDLE) == IDLE) ? true : false;
	player->is_reloading	= ((info_p->flags & RELOADING) == RELOADING) ? true : false;
	player->is_jet_packing	= ((info_p->flags & JETPACKING) == JETPACKING) ? true : false;
	player->is_jumping 		= ((info_p->flags & JUMPING) == JUMPING) ? true : false;

	player->run_animation_time		= info_p->run_animation_time;
	player->shoot_animation_time	= info_p->shoot_animation_time;
	player->idle_animation_time		= info_p->idle_animation_time;
	player->reload_animation_time 	= info_p->reload_animation_time;
	player->jump_animation_time		= info_p->jump_animation_time;
	player->die_animation_time		= info_p->die_animation_time;

	player->controller->controller->setWalkDirection(btVector3(info_p->velocityX, info_p->velocityY, info_p->velocityZ));
}


