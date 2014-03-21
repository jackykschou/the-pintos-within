#include "PlayerSpawner.h"

PlayerSpawner::PlayerSpawner(std::string tag, Scene* s) : GameObject(tag, s), positions(0)
{
}

PlayerSpawner::~PlayerSpawner()
{
}

void PlayerSpawner::startGame()
{
	Ogre::Vector3 position1 = positions[RAND_RANGE(0, positions.size())];

	PlayerCharacter* yourself = new PlayerCharacter(true, scene, "PixelMan.mesh",
                        position1.x, position1.y, position1.z,
                        0, 0, 0, 1,
                        10, 10, 10);

	Ogre::Vector3 position2 = positions[RAND_RANGE(0, positions.size())];
	while(position1 == position2)
	{
		position2 = positions[RAND_RANGE(0, positions.size())];
	}

	PlayerCharacter* enemy = new PlayerCharacter(false, scene, "PixelMan.mesh",
                        position2.x, position2.y, position2.z,
                        0, 0, 0, 1,
                        10, 10, 10);
}

void PlayerSpawner::addSpawnPoint(Ogre::Vector3 point)
{
	positions.push_back(point);
}

void PlayerSpawner::spawnPlayer(bool self)
{
	Ogre::Vector3 position = positions[RAND_RANGE(0, positions.size())];
	
	new PlayerCharacter(self, scene, "PixelMan.mesh",
            position.x, position.y, position.z,
            0, 0, 0, 1,
            10, 10, 10);
}

