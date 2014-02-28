#include "GameState.h"

namespace pt = boost::posix_time;

void GameState::reset() {
	score = 0;
	timeLeft = DEFAULT_CLOCK;
	_start = pt::second_clock::local_time();
}

void GameState::update() {
	if (timeLeft < 1) {
		GUIManager::instance()->showGameOverMenu();
	} else {
		pt::ptime now = pt::second_clock::local_time();
		pt::time_duration diff = now - _start;
		timeLeft = DEFAULT_CLOCK - diff.total_seconds();
	}
}
