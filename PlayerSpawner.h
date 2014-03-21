#ifndef __PlayerSpawner_h_
#define __PlayerSpawner_h_

#include "common.h"

#include "GameObject.h"
#include "PlayerCharacter.h"

class PlayerSpawner : GameObject
{
public:
	PlayerSpawner(std::string, Scene*);
	virtual ~PlayerSpawner();

	std::vector<Ogre::Vector3> positions;

	void startGame();
	void addSpawnPoint(Ogre::Vector3);
	void spawnPlayer(bool);
};

#endif