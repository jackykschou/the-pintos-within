#include "PlayerSpawner.h"

#include "NetworkManager.h"

PlayerSpawner::PlayerSpawner(std::string tag, Scene* s) : GameObject(tag, s), positions(), red_positions(), blue_positions()
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
			for(int i = 0; i < GameState::instance()->num_player; ++i)
			{	
				Ogre::Vector3 position;
				if(NetworkManager::instance()->player_team_id_map[i] == RED_TEAM)
				{
					do
					{
						position = red_positions[RAND_RANGE(0, red_positions.size())];
					}while(std::find(spawned_positions_red.begin(), spawned_positions_red.end(), position) != spawned_positions_red.end());
					spawned_positions_red.push_back(position);
				}
				else
				{
					do
					{
						position = blue_positions[RAND_RANGE(0, blue_positions.size())];
					}while(std::find(spawned_positions_blue.begin(), spawned_positions_blue.end(), position) != spawned_positions_blue.end());
					spawned_positions_blue.push_back(position);
				}

				spawnPlayer(position.x, position.y, position.z, i, NetworkManager::instance()->player_team_id_map[i]);

				NetworkManager::instance()->vital->setPlayerRespawn(position.x, position.y, position.z, i, NetworkManager::instance()->player_team_id_map[i]);
			}
		}
		else
		{
			std::vector<Ogre::Vector3> spawned_positions;
			for(int i = 0; i < GameState::instance()->num_player; ++i)
			{	
				Ogre::Vector3 position;
				do
				{
					position = positions[RAND_RANGE(0, positions.size())];
				}while(std::find(spawned_positions.begin(), spawned_positions.end(), position) != spawned_positions.end());
				spawned_positions.push_back(position);
				spawnPlayer(position.x, position.y, position.z, i, NetworkManager::instance()->player_team_id_map[i]);

				NetworkManager::instance()->vital->setPlayerRespawn(position.x, position.y, position.z, i, NetworkManager::instance()->player_team_id_map[i]);
			}
		}
	}
}

void PlayerSpawner::addSpawnPoint(Ogre::Vector3 point, uint32_t spawn_team_id)
{
	positions.push_back(point);

	if(spawn_team_id == RED_TEAM)
	{
		red_positions.push_back(point);
	}
	else
	{
		blue_positions.push_back(point);
	}
}

void PlayerSpawner::spawnPlayer(float x, float y, float z, uint32_t player_id, uint32_t team_id)
{
	bool self = false;

	if(player_id == NetworkManager::instance()->player_id)
		self = true;

	PlayerCharacter *player = new PlayerCharacter(self, scene, "PixelMan.mesh",
            x, y, z,
            0, 0, 0, 1,
            10, 10, 10,
            player_id);
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

	return position;
}

void PlayerSpawner::update()
{
	GameObject::update();
	if(NetworkManager::instance()->isServer() && GameState::instance()->isRunning())
	{
		if(GameState::instance()->player == NULL && GameState::instance()->game_mode == DEATHMATCH)
		{
			Ogre::Vector3 pos = GameState::instance()->spawner->spawnPlayer(NetworkManager::instance()->player_id);
			NetworkManager::instance()->vital->setPlayerRespawn(pos.x, pos.y, pos.z, 
				NetworkManager::instance()->player_id, NetworkManager::instance()->player_team_id_map[NetworkManager::instance()->player_id]);
		}
	}
}



