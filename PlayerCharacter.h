#ifndef __PlayerCharacter_h_
#define __PlayerCharacter_h_

#include "common.h"

#include "GameObject.h"
#include "FPSBoxController.h"
#include "PlayerBox.h"
#include "Mesh.h"
#include "Weapon.h"
#include "Debouncer.h"

#include <OgreAnimationState.h>

#define WEAPON_NUM 4

class Weapon;

class PlayerCharacter : GameObject
{
protected:
	void regen_health();

public:
	bool is_yourself;
	
	bool is_dead;
	bool is_shooting;
	bool is_moving;
	bool is_idle;
	bool is_reloading;
	bool is_jet_packing;
	bool is_jumping;

	float run_animation_time;
	float shoot_animation_time;
	float idle_animation_time;
	float reload_animation_time;
	float jump_animation_time;
	float head_animation_time;
	float die_animation_time;

	Ogre::AnimationState *running_animation_state;
	Ogre::AnimationState *idle_animation_state;
	Ogre::AnimationState *shooting_animation_states[WEAPON_NUM];
	Ogre::AnimationState *reload_animation_states[WEAPON_NUM];
	Ogre::AnimationState *jumping_animation_state;
	Ogre::AnimationState *head_animation_state;
	Ogre::AnimationState *die_animation_state;

	Ogre::AnimationState *current_reload_animation_state;
	Ogre::AnimationState *current_shooting_animation_state;


	Ogre::AnimationState *weapon_running_animation_state;
	Ogre::AnimationState *weapon_idle_animation_state;
	Ogre::AnimationState *weapon_shooting_animation_state;
	Ogre::AnimationState *weapon_reload_animation_state;
	Ogre::AnimationState *weapon_jumping_animation_state;
	Ogre::AnimationState *weapon_die_animation_state;

	int health;

	int health_regen;
	double health_regen_rate;

	FPSBoxController* controller;
	Mesh* mesh;
	Weapon* weapons[WEAPON_NUM];
	Weapon* current_weapon;

	Debouncer *health_regen_debouncer;

	PlayerCharacter(bool, Scene*, std::string,
	float, float, float, float, float, float, float,
	float, float, float);

	virtual ~PlayerCharacter();

	virtual void update();
	virtual void changeWeapon(int);
};

#endif // #ifndef __PlayerCharacter_h_