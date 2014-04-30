#include "GameState.h"
#include "PlayerSpawner.h"
#include "PlayerCharacter.h"
#include "NetworkManager.h"

namespace pt = boost::posix_time;

GameState::GameState()
{
	memset(&players, 0, sizeof(PlayerCharacter*) * MAX_PLAYER);
	memset(&player_pinto_seeds, 0, sizeof(bool) * MAX_PLAYER);
}

void GameState::reset() {
	score = 0;
	timeLeft = DEFAULT_CLOCK;
	_start = pt::second_clock::local_time();
	_running = false;
}

void GameState::start() {
	reset();
	_running = true;
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

  carrying_pinto_seed = false;
  
  spawner->startGame();
}

bool GameState::isRunning() {
	return _running;
}

void GameState::update() 
{
	if (!(timeLeft < 1 || !_running)) {
		pt::ptime now = pt::second_clock::local_time();
		pt::time_duration diff = now - _start;
		timeLeft = DEFAULT_CLOCK - diff.total_seconds();
	}



	//check for if game has finished
}
