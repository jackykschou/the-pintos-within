#include "GameState.h"
#include "PlayerSpawner.h"
#include "PlayerCharacter.h"

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
    spawner->startGame();
    GUIManager::instance()->hideGameOverMenu();
	AudioManager::instance()->playStartSound();
	GUIManager::instance()->hideWaitingMenu();
}

bool GameState::isRunning() {
	return _running;
}

void GameState::update() {
	if (!(timeLeft < 1 || !_running)) {
		pt::ptime now = pt::second_clock::local_time();
		pt::time_duration diff = now - _start;
		timeLeft = DEFAULT_CLOCK - diff.total_seconds();
	}
}
