#include "PlayerSpawner.h"

#include "NetworkManager.h"

PlayerSpawner::PlayerSpawner(std::string tag, Scene* s) : GameObject(tag, s), positions(0)
{
}

PlayerSpawner::~PlayerSpawner()
{
}

void PlayerSpawner::startGame()
{
	if(NetworkManager::instance()->isServer())
	{

	}
}

void PlayerSpawner::addSpawnPoint(Ogre::Vector3 point)
{
	positions.push_back(point);
}

void PlayerSpawner::spawnPlayer(float x, float y, float z, uint32_t player_id)
{
	bool self = false;

	if(player_id == NetworkManager::instance()->player_id)
		self = true;

	PlayerCharacter *player = new PlayerCharacter(self, scene, "PixelMan.mesh",
            x, y, z,
            0, 0, 0, 1,
            10, 10, 10,
            player_id);

	if(self)
	{
		GameState::instance()->player = player;
	}
}

Ogre::Vector3 PlayerSpawner::spawnPlayer(uint32_t player_id)
{
	Ogre::Vector3 position = positions[RAND_RANGE(0, positions.size())];
	
	bool self = false;

	if(player_id == NetworkManager::instance()->player_id)
	{
		self = true;
	}

	PlayerCharacter *player = new PlayerCharacter(self, scene, "PixelMan.mesh",
            position.x, position.y, position.z,
            0, 0, 0, 1,
            10, 10, 10,
            player_id);

	if(self)
	{
		GameState::instance()->player = player;
	}
	
	return position;
}



