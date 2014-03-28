#ifndef __HeartbeatPacket_h_
#define __HeartbeatPacket_h_

#include "common.h"

#include "PlayerCharacter.h"

struct Information
{
	char type;

	bool exist;

	float playerPosX;
	float playerPosY;
	float playerPosZ;

	float playerRotX;
	float playerRotY;
	float playerRotZ;
	float playerRotW;

	bool is_dead;
	bool is_shooting;
	bool is_moving;
	bool is_idle;
	bool is_reloading;
	bool is_jet_packing;
	bool is_jumping;	

	float run_animation_time;
	float shoot_animation_time;
	float idle_animation_time;
	float reload_animation_time;
	float jump_animation_time;
	float head_animation_time;
	float die_animation_time;
};

class HeartbeatPacket
{
public:
	struct Information info;
	HeartbeatPacket();

	void renewPacket(PlayerCharacter* player);
	void updatePlayer(PlayerCharacter*);
};

#endif // #ifndef __HeartbeatPacket_h_