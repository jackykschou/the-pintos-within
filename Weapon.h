#ifndef __Weapon_h_
#define __Weapon_h_

#include "common.h"

#include "Component.h"
#include "PlayerCharacter.h"
#include "Debouncer.h"

class PlayerCharacter;

class Weapon : Component
{
public:
	PlayerCharacter* player;

	int weapon_id;

	double reload_time;
	int shoot_cost;
	int current_mag_count;
	int max_mag_cap;
	int max_ammo;
	int current_ammo;
	double cooldown;

	bool reloading;

	Mesh* mesh;
	Debouncer* shoot_debouncer;

	Weapon(PlayerCharacter* player_p, std::string mesh_name, int id_p, double reload_time_p, 
			int shoot_cost_p, int max_mag_cap_p, int max_ammo_p, double cooldown_p);
	virtual ~Weapon();

	void shoot();
	virtual void shoot_hook() = 0;
	virtual void reload();
	virtual void update();

protected:
	boost::posix_time::ptime last_reload_time;

};

#endif // #ifndef __Weapon_h_