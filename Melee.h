#ifndef __Melee_h_
#define __Melee_h_

#include "common.h"

#include "GraphicsManager.h"
#include "PhysicsManager.h"
#include "Weapon.h"
#include "HitBox.h"

class Melee : public Weapon
{
public:
	double damage;
	double damage_radius;
	Melee(PlayerCharacter*, std::string, float, 
			float, float, float, float, float, float,
			float, float, float, PlayerBox*);

	virtual void shoot_hook();
};

#endif // #ifndef __Melee_h_
