#include "VitalPacket.h"

#include "NetworkManager.h"
#include "PlayerCharacter.h"
#include "PlayerSpawner.h"
#include "WeaponSpawner.h"

void VitalPacket::setDamage(int damage, uint32_t player_id)
{
	PlayerDamageInfo info;
	info.type = TAKEDAMAGE;
	info.player_id = NetworkManager::instance()->player_id;
	info.damage = damage;
	info.damaged_player_id = player_id;
	info.damage_dealer_id = NetworkManager::instance()->player_id;
	NetworkManager::instance()->send(&info, sizeof(PlayerDamageInfo), true);
}

void VitalPacket::receiveDamage(PlayerDamageInfo* info_p)
{
	if (info_p->player_id == NetworkManager::instance()->player_id || !GameState::instance()->isRunning())
		return;

	if( GameState::instance()->player != NULL &&
		info_p->damaged_player_id == NetworkManager::instance()->player_id &&
			GameState::instance()->players[info_p->damaged_player_id] &&
			!GameState::instance()->players[info_p->damaged_player_id]->is_dead)
	{
		if(GameState::instance()->player->health > 0)
		{
			GameState::instance()->player->health -= info_p->damage;

			//set score according to game mode
			if(GameState::instance()->player->health <= 0)
			{
				if(GameState::instance()->team_mode == FFA && (GameState::instance()->game_mode != PINTO))
				{
					NetworkManager::instance()->vital->setIncreaseScore(info_p->damage_dealer_id, 1, RED_TEAM);
				}
				else if((GameState::instance()->team_mode == TEAM) && (GameState::instance()->game_mode != PINTO))
				{
					LOG("TEAM MODE!");
					if(GameState::instance()->player->team_id == RED_TEAM)
					{
						NetworkManager::instance()->vital->setIncreaseScore(info_p->damage_dealer_id, 1, BLUE_TEAM);
					}
					else
					{
						NetworkManager::instance()->vital->setIncreaseScore(info_p->damage_dealer_id, 1, RED_TEAM);
					}
				}
				else
				{
					if(((GameState::instance()->players[info_p->damage_dealer_id] != NULL)
						&& (GameState::instance()->players[info_p->damage_dealer_id]->in_pinto_form))
						|| GameState::instance()->player_pinto_seeds[info_p->damage_dealer_id])
					{
						NetworkManager::instance()->vital->setIncreaseScore(info_p->damage_dealer_id, 10, RED_TEAM);
					}
					if(GameState::instance()->player->in_pinto_form)
					{
						if((GameState::instance()->players[info_p->damage_dealer_id] == NULL)
						|| (GameState::instance()->players[info_p->damage_dealer_id]->is_dead))
						{
							GameState::instance()->player_pinto_seeds[info_p->damage_dealer_id] = true;
						}
						else
						{
							GameState::instance()->players[info_p->damage_dealer_id]->changeToPinto();
						}
						NetworkManager::instance()->vital->setChangePinto(info_p->damage_dealer_id);
					}
				}
			}
		}

		AudioManager::instance()->playBlootSplat(Ogre::Vector3(GameState::instance()->player->transform->posX,
													GameState::instance()->player->transform->posY,
													GameState::instance()->player->transform->posZ));
	}
}

void VitalPacket::setPlayerRespawn(float posX, float posY, float posZ, uint32_t player_id, uint32_t team_id, uint32_t version, bool is_pinto)
{
	PlayerRespawnInfo info;
	info.type = PLAYER_RESPAWN;
	info.player_id = NetworkManager::instance()->player_id;
	info.player_respawn_id = player_id;
	info.playerX = posX;
	info.playerY = posY;
	info.playerZ = posZ;
	info.team_id = team_id;
	info.version = version;
	info.is_pinto = is_pinto;
	NetworkManager::instance()->send(&info, sizeof(PlayerRespawnInfo), true);
}

void VitalPacket::receivePlayerRespawn(PlayerRespawnInfo* info_p)
{
	if (info_p->player_id == NetworkManager::instance()->player_id || !GameState::instance()->isRunning())
		return;

	if(GameState::instance()->players[info_p->player_respawn_id] != NULL)
	{
		((GameObject*)(GameState::instance()->players[info_p->player_respawn_id]))->scene->removeGameObject((GameObject*)GameState::instance()->players[info_p->player_respawn_id]);
		delete GameState::instance()->players[info_p->player_respawn_id];
		GameState::instance()->players[info_p->player_respawn_id] = NULL;
	}

	GameState::instance()->spawner->spawnPlayer(info_p->playerX, info_p->playerY, info_p->playerZ, info_p->player_respawn_id, info_p->team_id, info_p->version, info_p->is_pinto);
}

void VitalPacket::setPlayerDie()
{
	PlayerDieInfo info;
	info.type = PLAYER_DIE;
	info.player_id = NetworkManager::instance()->player_id;
	NetworkManager::instance()->send(&info, sizeof(PlayerDieInfo), true);
}

void VitalPacket::receivePlayerDie(PlayerDieInfo* info_p)
{
	if (info_p->player_id == NetworkManager::instance()->player_id || !GameState::instance()->isRunning())
		return;
	
	if(GameState::instance()->players[info_p->player_id] != NULL)
	{
		((GameObject*)(GameState::instance()->players[info_p->player_id]))->scene->removeGameObject((GameObject*)GameState::instance()->players[info_p->player_id]);
		delete GameState::instance()->players[info_p->player_id];
		GameState::instance()->players[info_p->player_id] = NULL;
	}

	if(NetworkManager::instance()->isServer())
	{
		if(GameState::instance()->game_mode != ELIMINATION)
		{
			GameState::instance()->spawner->spawnPlayer(info_p->player_id, GameState::instance()->player_pinto_seeds[info_p->player_id]);
		}
	}
}

void VitalPacket::setChangeWeapon(uint32_t index)
{
	ChangeWeaponInfo info;
	info.type = WEAPON_CHANGE;
	info.player_id = NetworkManager::instance()->player_id;
	info.weapon_index = index;
	NetworkManager::instance()->send(&info, sizeof(ChangeWeaponInfo), true);
}

void VitalPacket::receiveChangeWeapon(ChangeWeaponInfo* info_p)
{
	if (info_p->player_id == NetworkManager::instance()->player_id || !GameState::instance()->isRunning())
		return;

	if(GameState::instance()->players[info_p->player_id] != NULL)
		GameState::instance()->players[info_p->player_id]->changeWeapon(info_p->weapon_index);
}

void VitalPacket::setSpawnWeapon(uint32_t id, float x, float y, float z, int pick_up_id, int pick_up_type)
{
	WeaponSpawnInfo info;
	info.type = WEAPON_SPAWN;
	info.player_id = NetworkManager::instance()->player_id;
	info.weapon_id = id;
	info.spawnX = x;
 	info.spawnY = y;
  	info.spawnZ = z;
  	info.pick_up_id = pick_up_id;
  	info.pick_up_type = pick_up_type;
	NetworkManager::instance()->send(&info, sizeof(WeaponSpawnInfo), true);
}

void VitalPacket::receiveSpawnWeapon(WeaponSpawnInfo* info)
{
	if (info->player_id == NetworkManager::instance()->player_id || !GameState::instance()->isRunning())
		return;
	
	GameState::instance()->weapon_spawner->spawnWeapon(info->spawnX, info->spawnY, info->spawnZ, info->weapon_id, info->pick_up_id, info->pick_up_type);
}

void VitalPacket::setPlayerFireSound()
{
	PlayFireSoundInfo info;
	info.type = PLAY_FIRE_SOUND;
	info.player_id = NetworkManager::instance()->player_id;

	NetworkManager::instance()->send(&info, sizeof(PlayFireSoundInfo), true);
}

void VitalPacket::receivePlayFireSound(PlayFireSoundInfo* info_p)
{
	if (info_p->player_id == NetworkManager::instance()->player_id || !GameState::instance()->isRunning())
		return;

	if(GameState::instance()->players[info_p->player_id] != NULL)
	{
		AudioManager::instance()->playWeaponFire(Ogre::Vector3(GameState::instance()->players[info_p->player_id]->transform->posX, 
																GameState::instance()->players[info_p->player_id]->transform->posY, 
																GameState::instance()->players[info_p->player_id]->transform->posZ),
												GameState::instance()->players[info_p->player_id]->current_weapon->weapon_id);
	}
}

void VitalPacket::setChangePinto(uint32_t to_pinto_player_id)
{
	ChangePintoInfo info;
	info.type = CHANGE_PINTO;
	info.player_id = NetworkManager::instance()->player_id;
	info.to_pinto_player_id = to_pinto_player_id;

	NetworkManager::instance()->send(&info, sizeof(ChangePintoInfo), true);
}

void VitalPacket::receiveChangePinto(ChangePintoInfo* info_p)
{
	if (info_p->player_id == NetworkManager::instance()->player_id || !GameState::instance()->isRunning())
		return;

	if(GameState::instance()->players[info_p->to_pinto_player_id] == NULL
		|| (GameState::instance()->players[info_p->to_pinto_player_id]->is_dead))
	{
		GameState::instance()->player_pinto_seeds[info_p->player_id] = true;
	}
	else
	{
		GameState::instance()->players[info_p->to_pinto_player_id]->changeToPinto();
	}
}

void VitalPacket::setIncreaseScore(uint32_t player_id, uint32_t amount, uint32_t team_id)
{
	if (NetworkManager::instance()->isServer()) {
		if((GameState::instance()->team_mode == TEAM && GameState::instance()->game_mode != PINTO))
		{
			if (team_id == GameState::instance()->team_id){
				GameState::instance()->score += amount;
				GameState::instance()->playerScores[player_id] += amount;
			}
		} else {
			GameState::instance()->playerScores[player_id] += amount;
		}
	}

	IncreaseScoreInfo info;
	info.type = INCREASE_SCORE;
	info.receive_player_id = player_id;
	info.amount = amount;
	info.receive_team_id = team_id;

	NetworkManager::instance()->send(&info, sizeof(IncreaseScoreInfo), true);
}

void VitalPacket::receiveIncreaseScore(IncreaseScoreInfo* info_p)
{
	if(!GameState::instance()->isRunning())
		return;

	if( (GameState::instance()->team_mode == TEAM && GameState::instance()->game_mode != PINTO))
	{
		if (info_p->receive_team_id == GameState::instance()->team_id)
			GameState::instance()->score += info_p->amount;
	}
	else if(info_p->receive_player_id == NetworkManager::instance()->player_id)
	{
		GameState::instance()->score += info_p->amount;
	}

	if (GameState::instance()->team_mode == TEAM && GameState::instance()->game_mode != PINTO) {
		LOG("TEAMMODE");
		
		if (NetworkManager::instance()->isServer()) {

			for(auto iter = GameState::instance()->playerConnections.begin();
	                 iter != GameState::instance()->playerConnections.end(); ++iter)
	        {
        		int id = iter->first;
        		LOG("SERVERMODE");
				int team = NetworkManager::instance()->player_team_id_map[info_p->receive_player_id];
	        	if (NetworkManager::instance()->player_team_id_map[id] == team) {
	        		GameState::instance()->playerScores[id] += info_p->amount;
	        	}
	        } 
        }
    	if (GameState::instance()->team_id == info_p->receive_team_id)
        	GameState::instance()->score += info_p->amount;
	} else {
		LOG("INCREMENTING "<<GameState::instance()->playerScores[info_p->receive_player_id]<< " BY "<<info_p->amount);
		GameState::instance()->playerScores[info_p->receive_player_id] += info_p->amount;
	}
}

void VitalPacket::setTimeLeft(uint32_t time_left)
{
	TimeLeftInfo info;
	info.type = TIME_LEFT;
	info.player_id = NetworkManager::instance()->player_id;
	info.time_left = GameState::instance()->timeLeft;

	NetworkManager::instance()->send(&info, sizeof(TimeLeftInfo), true);
}

void VitalPacket::receiveTimeLeft(TimeLeftInfo* info_p)
{
	if (info_p->player_id == NetworkManager::instance()->player_id || !GameState::instance()->isRunning())
		return;

	GameState::instance()->timeLeft = info_p->time_left;
}

void VitalPacket::setChangeHair(uint32_t index)
{
	ChangeHairInfo info;
	info.type = HAIR_CHANGE;
	info.player_id = NetworkManager::instance()->player_id;
	info.hair_index = index;
	NetworkManager::instance()->send(&info, sizeof(ChangeHairInfo), true);
}

void VitalPacket::receiveChangeHair(ChangeHairInfo* info_p)
{
	if (info_p->player_id == NetworkManager::instance()->player_id || !GameState::instance()->isRunning())
		return;

	if(GameState::instance()->players[info_p->player_id] != NULL)
		GameState::instance()->players[info_p->player_id]->changeHair(info_p->hair_index);
}
