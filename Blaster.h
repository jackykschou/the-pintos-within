#ifndef __Blaster_h_
#define __Blaster_h_

#include "common.h"

#include "GraphicsManager.h"
#include "PhysicsManager.h"
#include "Weapon.h"
#include "HitBox.h"

class Blaster : public Weapon
{
public:
	double damage;
	double shoot_distance;
	Blaster(PlayerCharacter*, std::string, float, 
			float, float, float, float, float, float,
			float, float, float, PlayerBox*);
	virtual ~Blaster();

	bool is_charging;
	float charge_scale;
	float charge_rate;
	float blast_radius;
	int max_ammot_cost;

	virtual void shoot_hook();
	virtual void shoot();

	Debouncer *charge_sound_debouncer;
};

#endif // #ifndef __Blaster_h_
