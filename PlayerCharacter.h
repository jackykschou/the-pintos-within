#ifndef __PlayerCharacter_h_
#define __PlayerCharacter_h_

#include "common.h"

#include "GameObject.h"
#include "FPSBoxController.h"
#include "Mesh.h"
#include "Weapon.h"

#include <OgreAnimationState.h>

#define WEAPON_NUM 4

class Weapon;

class PlayerCharacter : GameObject
{
protected:

public:
	FPSBoxController* controller;
	Mesh* mesh;
	Weapon* weapons[WEAPON_NUM];
	Weapon* current_weapon;

	Ogre::AnimationState *animation_state;

	PlayerCharacter(std::string, Scene*, 
	int, int, std::string,
	float, float, float, float, float, float, float,
	float, float, float);

	virtual ~PlayerCharacter();

	virtual void update();
};

#endif // #ifndef __PlayerCharacter_h_