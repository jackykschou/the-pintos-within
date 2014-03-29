#ifndef __Pistol_h_
#define __Pistol_h_

#include "common.h"

#include "GraphicsManager.h"
#include "PhysicsManager.h"
#include "Weapon.h"
#include "HitBox.h"

class Pistol : public Weapon
{
public:
	double damage;
	double shoot_distance;
	Pistol(PlayerCharacter*, std::string, float, 
			float, float, float, float, float, float,
			float, float, float, PlayerBox*);


	virtual void shoot_hook();

	Debouncer *shoot_sound_debouncer;
};

#endif // #ifndef __Pistol_h_
