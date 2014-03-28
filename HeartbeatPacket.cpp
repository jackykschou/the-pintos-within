#include "HeartbeatPacket.h"

#include "PlayerCharacter.h"
#include "PhysicsManager.h"

HeartbeatPacket::HeartbeatPacket()
{
	info.type = HEARTBEATPACK;
}

void HeartbeatPacket::clear()
{
	memset(&info, 0, sizeof(HeartBeatInfo));
	info.type = HEARTBEATPACK;
}

void HeartbeatPacket::renewPlayerInfo(PlayerCharacter* player)
{
	info.flags = 0;

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
	info.head_animation_time	= player->head_animation_time;
	info.die_animation_time		= player->die_animation_time;
}

void HeartbeatPacket::updatePlayer(PlayerCharacter* player)
{
	Transform* tran = ((GameObject*)player)->getComponent<Transform>();

	tran->posX = info.playerPosX;
	tran->posY = info.playerPosY;
	tran->posZ = info.playerPosZ;

	tran->rotX = info.playerRotX;
	tran->rotY = info.playerRotY;
	tran->rotZ = info.playerRotZ;
	tran->rotW = info.playerRotW;

	player->is_dead			= ((info.flags & DEAD) == DEAD) ? true : false;
	player->is_shooting		= ((info.flags & SHOOTING) == SHOOTING) ? true : false;
	player->is_moving		= ((info.flags & MOVING) == MOVING) ? true : false;
	player->is_idle			= ((info.flags & IDLE) == IDLE) ? true : false;
	player->is_reloading	= ((info.flags & RELOADING) == RELOADING) ? true : false;
	player->is_jet_packing	= ((info.flags & JETPACKING) == JETPACKING) ? true : false;
	player->is_jumping 		= ((info.flags & JUMPING) == JUMPING) ? true : false;

	player->run_animation_time		= info.run_animation_time;
	player->shoot_animation_time	= info.shoot_animation_time;
	player->idle_animation_time		= info.idle_animation_time;
	player->reload_animation_time 	= info.reload_animation_time;
	player->jump_animation_time		= info.jump_animation_time;
	player->head_animation_time		= info.head_animation_time;
	player->die_animation_time		= info.die_animation_time;

	player->controller->controller->setWalkDirection(btVector3(info.velocityX, info.velocityY, info.velocityZ));
}


