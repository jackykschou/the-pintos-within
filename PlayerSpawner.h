#ifndef __PlayerSpawner_h_
#define __PlayerSpawner_h_

#include "common.h"

#include "GameObject.h"
#include "PlayerCharacter.h"
#include "NetworkManager.h"

class PlayerSpawner : GameObject
{
public:
	PlayerSpawner(std::string, Scene*);
	virtual ~PlayerSpawner();

	std::vector<Ogre::Vector3> positions;
	std::vector<Ogre::Vector3> red_positions;
	std::vector<Ogre::Vector3> blue_positions;

	void startGame();
	void addSpawnPoint(Ogre::Vector3, uint32_t);
	void spawnPlayer(float, float, float, uint32_t, uint32_t, uint32_t, bool);
	Ogre::Vector3 spawnPlayer(uint32_t, bool);

	virtual void update();
};

#endif