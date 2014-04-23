#ifndef __ShotGun_h_
#define __ShotGun_h_

#include "common.h"

#include "GraphicsManager.h"
#include "PhysicsManager.h"
#include "Weapon.h"
#include "HitBox.h"

class ShotGun : public Weapon
{
public:
	double damage;
	double shoot_distance;
	ShotGun(PlayerCharacter*, std::string, float, 
			float, float, float, float, float, float,
			float, float, float, PlayerBox*);

	virtual void shoot_hook();
};

#endif // #ifndef __ShotGun_h_
