#ifndef __WeaponSpawner_h_
#define __WeaponSpawner_h_

#include "common.h"

#include "GameObject.h"
#include "PlayerCharacter.h"
#include "NetworkManager.h"

class WeaponSpawner : GameObject
{
public:
	WeaponSpawner(std::string, Scene*);
	virtual ~WeaponSpawner();

	std::vector<Ogre::Vector3> positions;

	void startGame();
	void addSpawnPoint(Ogre::Vector3);
	void spawnPlayer(bool);

	virtual void update();
};

#endif