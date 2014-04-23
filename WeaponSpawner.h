#ifndef __WeaponSpawner_h_
#define __WeaponSpawner_h_

#include "common.h"

#include "GameObject.h"
#include "PlayerCharacter.h"
#include "NetworkManager.h"

class Debouncer;

class WeaponSpawner : GameObject
{
public:
	
	uint32_t id_assigner;
	uint32_t current_weapon_count;

	std::vector<Ogre::Vector3> positions;
	Debouncer* spawn_debouncer;

	WeaponSpawner(std::string, Scene*);
	virtual ~WeaponSpawner();

	void startGame();
	void addSpawnPoint(Ogre::Vector3);
	void spawnWeapon(float, float, float, uint32_t);
	Ogre::Vector3 spawnWeapon();
	Ogre::Vector3 spawnPlayer(uint32_t);

	virtual void update();
};

#endif