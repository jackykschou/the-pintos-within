#ifndef __Application_h_
#define __Application_h_

#include "TheEngine.h"
#include "DotSceneLoader.h"

#define CUBE_SCALE 1

#define BIT(x) (1<<(x))

enum collisiontypes {
    COL_NOTHING = 0,
    COL_STATIC = BIT(0),
    COL_CHARACTER = BIT(1)
};

#define STATIC_COLLIDER_WITH (COL_STATIC)
#define CHARACTER_COLLIDER_WITH (COL_STATIC | COL_CHARACTER)


#endif // #ifndef __Application_h_