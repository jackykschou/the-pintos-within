#ifndef __Weapon_h_
#define __Weapon_h_

#include "common.h"

#include "Component.h"
#include "PlayerCharacter.h"
#include "Debouncer.h"
#include "PlayerBox.h"

class PlayerCharacter;
class PlayerBox;

class Weapon : protected Component
{
public:
	PlayerCharacter* player;
	PlayerBox* shoot_pos;

	Ogre::AnimationState *running_animation_state;
	Ogre::AnimationState *idle_animation_state;
	Ogre::AnimationState *shooting_animation_state;
	Ogre::AnimationState *reload_animation_state;
	Ogre::AnimationState *jumping_animation_state;
	Ogre::AnimationState *die_animation_state;

	float reload_timer;
	float shoot_timer;

	int weapon_id;

	double reload_time;
	int shoot_cost;
	int current_mag_count;
	int max_mag_cap;
	int max_ammo;
	int current_ammo;
	double cooldown;

	bool is_shooting;
	bool is_reloading;

	Ogre::Entity* entity;
  Ogre::SceneNode* node;
  
	Weapon(PlayerCharacter*, std::string, int, double, 
			int, int, int, double, float, 
			float, float, float, float, float, float,
			float, float, float, PlayerBox*);
	virtual ~Weapon();

	void shoot();
	virtual void shoot_hook() = 0;
	virtual void reload();
	virtual void update();
	virtual void switchToThisWeapon();

};

#endif // #ifndef __Weapon_h_