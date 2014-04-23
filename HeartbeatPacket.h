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

class PlayerCharacter;

struct HeartBeatInfo
{
	char type;

	uint32_t player_id;

	uint32_t flags;

	float playerPosX;
	float playerPosY;
	float playerPosZ;

	float playerRotX;
	float playerRotY;
	float playerRotZ;
	float playerRotW;

	float velocityX;
	float velocityY;
	float velocityZ;

	float run_animation_time;
	float shoot_animation_time;
	float idle_animation_time;
	float reload_animation_time;
	float jump_animation_time;
	float die_animation_time;
};

class HeartbeatPacket
{
public:
	struct HeartBeatInfo info;
	HeartbeatPacket();

	void renewPlayerInfo(PlayerCharacter*);
	void updatePlayer(HeartBeatInfo*, PlayerCharacter*);
	void clear();
};

#endif // #ifndef __HeartbeatPacket_h_