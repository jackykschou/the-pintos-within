#include "HeartbeatPacket.h"

HeartbeatPacket::HeartbeatPacket()
{
	info.type = HEARTBEATPACK;
	info.exist = false;
}

void HeartbeatPacket::renewPacket(PlayerCharacter* player)
{
	info.exist = true;

	Transform* tran = ((GameObject*)player)->getComponent<Transform>();

	info.playerPosX = tran->posX;
	info.playerPosY = tran->posY;
	info.playerPosZ = tran->posZ;

	info.playerRotX = tran->rotX;
	info.playerRotY = tran->rotY;
	info.playerRotZ = tran->rotZ;
	info.playerRotW = tran->rotW;

	info.is_dead 		= player->is_dead;
	info.is_shooting 	= player->is_shooting;
	info.is_moving 		= player->is_moving;
	info.is_idle 		= player->is_idle;
	info.is_reloading 	= player->is_reloading;
	info.is_jet_packing = player->is_jet_packing;

	info.run_animation_time 	= player->run_animation_time;
	info.shoot_animation_time 	= player->shoot_animation_time;
	info.idle_animation_time 	= player->idle_animation_time;
	info.reload_animation_time 	= player->reload_animation_time;
}

void HeartbeatPacket::updatePlayer(PlayerCharacter* player)
{
	info.exist = true;

	Transform* tran = ((GameObject*)player)->getComponent<Transform>();

	tran->posX = info.playerPosX;
	tran->posY = info.playerPosY;
	tran->posZ = info.playerPosZ;

	tran->rotX = info.playerRotX;
	tran->rotY = info.playerRotY;
	tran->rotZ = info.playerRotZ;
	tran->rotW = info.playerRotW;

	player->is_dead			= info.is_dead; 		
	player->is_shooting		= info.is_shooting; 	
	player->is_moving;		= info.is_moving; 		
	player->is_idle			= info.is_idle; 		
	player->is_reloading	= info.is_reloading; 	
	player->is_jet_packing	= info.is_jet_packing; 

	player->run_animation_time		= info.run_animation_time;
	player->shoot_animation_time	= info.shoot_animation_time;
	player->idle_animation_time		= info.idle_animation_timee;
	player->reload_animation_time 	= info.reload_animation_time;
}


