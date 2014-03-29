#ifndef __Utility_h_
#define __Utility_h_

// given an arbitrarily-sized struct, MEMALIGNED_SIZE ensures
// that the size of the struct is a multiple of a word boundary
// (16 bytes on linux) to prevent memcpy from dying on alignment
//
// XXX: If I wasn't such a dick about using pointer arithmetic,
// we wouldn't have to worry about this -joev
#define MEMALIGNED_SIZE(struct) \
  ((sizeof(struct)/16)+1)*16

#define CHANCE(chance) \
  ((chance * 10000) >= (rand() % 10000 + 1))

#define RAND (((float) rand()) / (float) RAND_MAX)

#define RAND_RANGE(from, to) \
  ((rand() % to) + from)

#define CUBE_SCALE 1

#define BIT(x) (1<<(x))

enum collisiontypes {
    COL_NOTHING = 0,
    COL_STATIC = BIT(0),
    COL_CHARACTER = BIT(1),
    COL_BALL = BIT(2),
    COL_BULLET = BIT(3),
    COL_PICKUP = BIT(4),
    COL_HIT_BOX = BIT(5)
};

#define STATIC_COLLIDER_WITH (COL_BALL | COL_CHARACTER | COL_BULLET)
#define CHARACTER_COLLIDER_WITH (COL_STATIC | COL_CHARACTER)
#define BALL_COLLIDER_WITH (COL_STATIC | COL_CHARACTER | COL_BALL)
#define COL_BULLET_COLLIDER_WITH (COL_STATIC | COL_BULLET)
#define COL_HIT_BOX_COLLIDER_WITH (COL_BULLET)

#define HEARTBEATPACK 1
#define ASSIGNPLAYERID 2
#define VITALPACK 3
#define PARTICLEPACK 4
#define JOINGAME 5

#define WALL "Wall"
#define FLOORGRASS "FloorGrass"
#define FLOORTILE "FloorTiled"
#define PLATFORMDIRT "PlatformDirt"
#define PLATFORMGREY "PlatformGrey"
#define PLAYERSPAWNER "Spawner.Player.005"
#define LIGHT "Sun"
#define SKY "SkyHemi"

#define MAX_PLAYER 10

#define MAIN_MENU 1
#define GAME_MODE 2

#endif // #ifndef __Utility_h_
