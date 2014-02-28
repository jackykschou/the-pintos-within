#include "GameState.h"

namespace pt = boost::posix_time;

void GameState::reset() {
	score = 0;
	timeLeft = DEFAULT_CLOCK;
	_start = pt::second_clock::local_time();
	_running = false;
}

void GameState::start() {
	_running = true;
}

bool GameState::running() {
	return _running;
}

void GameState::update() {
	if (timeLeft < 1 || !_running) {
		GUIManager::instance()->showGameOverMenu();
	} else {
		pt::ptime now = pt::second_clock::local_time();
		pt::time_duration diff = now - _start;
		timeLeft = DEFAULT_CLOCK - diff.total_seconds();
	}
}
