#ifndef __Pistol_h_
#define __Pistol_h_

#include "common.h"

#include "GraphicsManager.h"
#include "PhysicsManager.h"
#include "Weapon.h"
#include "HitBox.h"

class Pistol : Weapon
{
public:
	double damage;
	double shoot_distance;
protected:
	Pistol(PlayerCharacter*, std::string, float, 
			float, float, float, float, float, float,
			float, float, float, PlayerBox*);


	virtual void shoot_hook();
};

#endif // #ifndef __Pistol_h_
