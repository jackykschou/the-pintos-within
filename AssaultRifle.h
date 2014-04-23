#ifndef __AssaultRifle_h_
#define __AssaultRifle_h_

#include "common.h"

#include "GraphicsManager.h"
#include "PhysicsManager.h"
#include "Weapon.h"
#include "HitBox.h"

class AssaultRifle : public Weapon
{
public:
	double damage;
	double shoot_distance;
	AssaultRifle(PlayerCharacter*, std::string, float, 
			float, float, float, float, float, float,
			float, float, float, PlayerBox*);

	virtual void shoot();
	virtual void shoot_hook();
};

#endif // #ifndef __AssaultRifle_h_
