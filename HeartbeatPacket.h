#ifndef __HeartbeatPacket_h_
#define __HeartbeatPacket_h_

#include "common.h"

#define DEAD 1
#define SHOOTING 2
#define MOVING 4
#define IDLE 8
#define RELOADING 16
#define JETPACKING 32
#define JUMPING 64

#include "PlayerCharacter.h"

struct HeartBeatInfo
{
	char type;

	float playerPosX;
	float playerPosY;
	float playerPosZ;

	float playerRotX;
	float playerRotY;
	float playerRotZ;
	float playerRotW;

	int flags;

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
	struct HeartBeatInfo info;
	HeartbeatPacket();

	void renewPacket(PlayerCharacter* player);
	void updatePlayer(PlayerCharacter*);
};

#endif // #ifndef __HeartbeatPacket_h_