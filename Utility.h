#ifndef __Utility_h_
#define __Utility_h_

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

#define STATIC_COLLIDER_WITH (COL_BALL | COL_CHARACTER)
#define CHARACTER_COLLIDER_WITH (COL_STATIC | COL_CHARACTER)
#define BALL_COLLIDER_WITH (COL_STATIC | COL_CHARACTER | COL_BALL)
#define COL_BULLET_COLLIDER_WITH (COL_STATIC | COL_CHARACTER)
#define COL_HIT_BOX_COLLIDER_WITH (COL_BULLET)

#define HEARTBEATPACK 1
#define JOINPACK 2
#define ACTIONPACK 3

#endif // #ifndef __Utility_h_