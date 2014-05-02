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

	std::vector<Ogre::Vector3> free_positions;
	Ogre::Vector3 red_position;
	Ogre::Vector3 blue_position;

	void startGame();
	void addFreeSpawnPoint(Ogre::Vector3);
	void addBlueSpawnPoint(Ogre::Vector3);
	void addRedSpawnPoint(Ogre::Vector3);
	void spawnPlayer(float, float, float, uint32_t, uint32_t, uint32_t, bool);
	Ogre::Vector3 spawnPlayer(uint32_t, bool);

	virtual void update();
};

#endif