#include "GameState.h"
#include "PlayerSpawner.h"
#include "PlayerCharacter.h"
#include "NetworkManager.h"

namespace pt = boost::posix_time;

GameState::GameState()
{
	memset(&players, 0, sizeof(PlayerCharacter*) * MAX_PLAYER);
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
  if(NetworkManager::instance()->isServer())
  {
  	PlayerNumInfo info;
		info.type = PLAYERNUM;
		info.num_player = num_player;
  	NetworkManager::instance()->send(&info, sizeof(PlayerNumInfo), true);
  }
  spawner->startGame();
}

bool GameState::isRunning() {
	return _running;
}

void GameState::update() {
	if (_running) {
		pt::ptime now = pt::second_clock::local_time();
		pt::time_duration diff = now - _start;
		timeLeft = DEFAULT_CLOCK - diff.total_seconds();
	}
	checkIfGameOver();

	if (!_running) {
		// perform the game over logic
		if (NetworkManager::instance()->isServer()) {
			
		}
		// NetworkManager::instance()->declareGameOver();
		GuiManager::instance()->ShowGameOver();
	}
}

bool GameState::isMoreThanOnePlayerAlive() {
	int alive = 0
	for (int i = 0; i < MAX_PLAYER; i++) {
		if (players[i] && !players[i]->is_dead) {
			alive++;
		}
		if (alive>1) break;
	}
	return (alive>1);
}

void GameState::checkIfGameOver() {
	switch (game_mode) {
		case ELIMINATION:

			if (!isMoreThanOnePlayerAlive()) {
				_running = false;
			}

			break;

		case DEATHMATCH:
		case PINTO:

			if (timeLeft < 1) {
				_running = false;
			}

			break;
	}
}