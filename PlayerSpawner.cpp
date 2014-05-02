#include "PlayerSpawner.h"
#include "NetworkManager.h"

#define SPAWN_RAND_RADIUS_MAX 50

PlayerSpawner::PlayerSpawner(std::string tag, Scene* s) : GameObject(tag, s), free_positions()
{
}

PlayerSpawner::~PlayerSpawner()
{
}

void PlayerSpawner::startGame()
{
	if(NetworkManager::instance()->isServer())
	{
		if(GameState::instance()->team_mode == TEAM && GameState::instance()->game_mode != PINTO)
		{
			std::vector<Ogre::Vector3> spawned_positions_blue;
			std::vector<Ogre::Vector3> spawned_positions_red;
			for(std::map<int,bool>::iterator iter = GameState::instance()->playerConnections.begin();
				iter != GameState::instance()->playerConnections.end(); ++iter)
		    {
		        int i = iter->first;	
				uint32_t version = RAND_RANGE(0, 5);
				float rand_x = RAND_RANGE(0, SPAWN_RAND_RADIUS_MAX) - 50;
				float rand_z = RAND_RANGE(0, SPAWN_RAND_RADIUS_MAX) - 50;
				Ogre::Vector3 position;
				if(NetworkManager::instance()->player_team_id_map[i] == RED_TEAM)
				{
					position = red_position;
					position.x = position.x + rand_x;
					position.z = position.z + rand_z;
				}
				else
				{
					position = blue_position;
					position.x = position.x + rand_x;
					position.z = position.z + rand_z;
				}

				spawnPlayer(position.x, position.y, position.z, i, NetworkManager::instance()->player_team_id_map[i], version, false);

				NetworkManager::instance()->vital->setPlayerRespawn(position.x, position.y, position.z, i, NetworkManager::instance()->player_team_id_map[i], version, false);
			}
		}
		else
		{
			uint32_t pinto_index = 100;

			if(GameState::instance()->game_mode == PINTO)
			{
				pinto_index = RAND_RANGE(0, GameState::instance()->num_player);
			}

			std::vector<Ogre::Vector3> spawned_positions;
			for(int i = 0; i < GameState::instance()->num_player; ++i)
			{	
				uint32_t version = RAND_RANGE(0, 5);
				Ogre::Vector3 position;
				do
				{
					position = free_positions[RAND_RANGE(0, free_positions.size())];
				}while(std::find(spawned_positions.begin(), spawned_positions.end(), position) != spawned_positions.end());
				spawned_positions.push_back(position);

				bool is_pinto = false;

				if(i == pinto_index)
				{
					is_pinto = true;
				}

				spawnPlayer(position.x, position.y, position.z, i, NetworkManager::instance()->player_team_id_map[i], version, is_pinto);
				NetworkManager::instance()->vital->setPlayerRespawn(position.x, position.y, position.z, i, NetworkManager::instance()->player_team_id_map[i], version, is_pinto);
			}
		}
	}
}

void PlayerSpawner::addFreeSpawnPoint(Ogre::Vector3 point)
{
	free_positions.push_back(point);
}

void PlayerSpawner::addBlueSpawnPoint(Ogre::Vector3 point)
{
	blue_position = point; 
	free_positions.push_back(point);
}

void PlayerSpawner::addRedSpawnPoint(Ogre::Vector3 point)
{
	red_position = point;
	free_positions.push_back(point);
}

void PlayerSpawner::spawnPlayer(float x, float y, float z, uint32_t player_id, uint32_t team_id, uint32_t version, bool is_pinto)
{
	bool self = false;

	if(player_id == NetworkManager::instance()->player_id)
		self = true;

	PlayerCharacter *player = new PlayerCharacter(self, scene, "PixelMan.mesh",
            x, y, z,
            0, 0, 0, 1,
            10, 10, 10,
            player_id, version, is_pinto);
	player->team_id = team_id;
	if(player->team_id == BLUE_TEAM)
	{
		player->switchToBlueTeam();
	}

	GameState::instance()->players[player_id] = player;

	if(self)
	{
		GameState::instance()->player = player;
	}
}

Ogre::Vector3 PlayerSpawner::spawnPlayer(uint32_t player_id, bool is_pinto)
{
	Ogre::Vector3 position;
	if(GameState::instance()->team_mode == FFA || GameState::instance()->game_mode == PINTO)
	{
		position = free_positions[RAND_RANGE(0, free_positions.size())];
	}
	else
	{
		float rand_x = RAND_RANGE(0, SPAWN_RAND_RADIUS_MAX) - 50;
		float rand_z = RAND_RANGE(0, SPAWN_RAND_RADIUS_MAX) - 50;
		if(NetworkManager::instance()->player_team_id_map[player_id] == RED_TEAM)
		{
			position = red_position;
			position.x = position.x + rand_x;
			position.z = position.z + rand_z;
		}
		else
		{
			position = blue_position;
			position.x = position.x + rand_x;
			position.z = position.z + rand_z;
		}
	}
	
	bool self = false;

	if(player_id == NetworkManager::instance()->player_id)
	{
		self = true;
	}

	uint32_t version = RAND_RANGE(0, 5);

	PlayerCharacter *player = new PlayerCharacter(self, scene, "PixelMan.mesh",
            position.x, position.y, position.z,
            0, 0, 0, 1,
            10, 10, 10,
            player_id, version, is_pinto);

	player->team_id = NetworkManager::instance()->player_team_id_map[NetworkManager::instance()->player_id];
	if(player->team_id == BLUE_TEAM)
	{
		player->switchToBlueTeam();
	}

	GameState::instance()->players[player_id] = player;

	if(self)
	{
		GameState::instance()->player = player;
	}

	NetworkManager::instance()->vital->setPlayerRespawn(position.x, position.y, position.z, 
				player_id, NetworkManager::instance()->player_team_id_map[player_id], version, is_pinto);

	return position;
}

void PlayerSpawner::update()
{
	GameObject::update();
	if(NetworkManager::instance()->isServer() && GameState::instance()->isRunning())
	{
		if(GameState::instance()->player == NULL && GameState::instance()->game_mode != ELIMINATION)
		{
			GameState::instance()->spawner->spawnPlayer(NetworkManager::instance()->player_id, GameState::instance()->player_pinto_seeds[NetworkManager::instance()->player_id]);
		}
	}
}



