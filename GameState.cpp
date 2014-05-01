#include "GameState.h"
#include "PlayerSpawner.h"
#include "PlayerCharacter.h"
#include "NetworkManager.h"

namespace pt = boost::posix_time;

GameState::GameState()
{
	memset(&players, 0, sizeof(PlayerCharacter*) * MAX_PLAYER);
	memset(&player_pinto_seeds, 0, sizeof(bool) * MAX_PLAYER);
	score = 0;
	timeLeft = DEFAULT_CLOCK;
	_start = pt::second_clock::local_time();
	_running = false;
}

void GameState::reset() {
	score = 0;
	timeLeft = DEFAULT_CLOCK;
	_start = pt::second_clock::local_time();
	_running = false;
}

void GameState::start() 
{
	reset();

	num_player_left_elimination = GameState::instance()->num_player;
	
    //GUIManager::instance()->hideGameOverMenu();
	// AudioManager::instance()->playStartSound();
	//GUIManager::instance()->hideWaitingMenu();

  if(NetworkManager::instance()->isServer())
  {
  	PlayerNumInfo info;
		info.type = PLAYERNUM;
		info.num_player = num_player;
  	NetworkManager::instance()->send(&info, sizeof(PlayerNumInfo), true);
  }


  SceneManager::instance()->changeCurrentScene(THEGAUNTLET);
  // SceneManager::instance()->changeCurrentScene(GameState::instance()->current_map);

  carrying_pinto_seed = false;
  
  spawner->startGame();

  _running = true;
}

bool GameState::isRunning() {
	return _running;
}

void GameState::update() 
{
	if (NetworkManager::instance()->isServer()) {
		if (!(timeLeft < 1 || !_running) && (GameState::instance()->game_mode != ELIMINATION))
		{
			pt::ptime now = pt::second_clock::local_time();
			pt::time_duration diff = now - _start;

			int before = timeLeft;
			timeLeft = DEFAULT_CLOCK - diff.total_seconds();

			if(before != timeLeft)
			{
				NetworkManager::instance()->vital->setTimeLeft(timeLeft);
			}
		}
		else if(timeLeft < 1 && _running && (GameState::instance()->game_mode != ELIMINATION))
		{
			//display end game message
			//hang for some time
			//send message to end the game to return to main menu
		}
		else if(_running && GameState::instance()->game_mode == ELIMINATION)
		{
			if(num_player_left_elimination <= 1)
			{
				//display end game message
				//hang for some time
				//send message to end the game to return to main menu
			}
		}
	}
}
