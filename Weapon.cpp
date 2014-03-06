#include "Weapon.h"

Weapon::Weapon(PlayerCharacter* player_p, std::string mesh_name, int id_p, double reload_time_p, 
	int shoot_cost_p, int max_mag_cap_p, int max_ammo_p, double cooldown_p) : Component((GameObject*)player)
{
	player = player_p;

	current_mag_count = 0;
	current_ammo = 0;
	reloading = false;

	shoot_debouncer = new Debouncer(cooldown_p * 1000);
}

Weapon::~Weapon()
{
	delete shoot_debouncer;
}


void Weapon::shoot()
{
	if((player->current_weapon == this) && current_mag_count >= shoot_cost && !reloading)
	{
		if(shoot_debouncer->run(NULL))
		{
			current_mag_count -= shoot_cost;
			shoot_hook();
		}
	}
}

void Weapon::reload()
{
	if((player->current_weapon == this) && !reloading && (current_mag_count != max_mag_cap) && (current_ammo != 0))
	{
		last_reload_time = boost::posix_time::second_clock::local_time();
		reloading = true;
	}
}

void Weapon::update()
{
	Component::update();
	if(reloading)
	{
		boost::posix_time::ptime now;
		now = boost::posix_time::second_clock::local_time();
		if((now - last_reload_time).total_milliseconds() >= (reload_time * 1000)) 
		{
			reloading = false;
			current_mag_count = (current_ammo >= max_mag_cap) ? max_mag_cap : current_ammo;
			current_ammo = (current_ammo >= max_mag_cap) ? (current_ammo - max_mag_cap) : 0;
		}
	}
}
