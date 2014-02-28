#ifndef __Application_h_
#define __Application_h_

#include "TheEngine.h"
#include "DotSceneLoader.h"

#define CUBE_SCALE 1

#define BIT(x) (1<<(x))

enum collisiontypes {
    COL_NOTHING = 0,
    COL_STATIC = BIT(0),
    COL_CHARACTER = BIT(1),
    COL_BALL = BIT(2)
};

#define STATIC_COLLIDER_WITH (COL_BALL | COL_CHARACTER)
#define CHARACTER_COLLIDER_WITH (COL_STATIC | COL_CHARACTER)
#define BALL_COLLIDER_WITH (COL_STATIC | COL_CHARACTER | COL_BALL)

#define WALL "Wall"
#define FLOOR "Floor"
#define PLATFORM "Platform"
#define PLAYER "Spawner.Player"
#define BALL_SPAWNER "Spawner.Ball"
#define LIGHT "Lam"



#endif // #ifndef __Application_h_