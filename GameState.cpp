#include "GameState.h"
#include "PlayerSpawner.h"
#include "PlayerCharacter.h"
#include "NetworkManager.h"

namespace pt = boost::posix_time;

GameState::GameState()
{
	score = 0;
	_start = pt::second_clock::local_time();
	_running = false;
}

void GameState::reset() 
{
	score = 0;
	timeLeft = originalTime;
	_start = pt::second_clock::local_time();
	_running = false;
	players.clear();
}

void GameState::start() 
{
	reset();

	num_player_left_elimination = GameState::instance()->num_player;

	_running = true;

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
}

bool GameState::isRunning() {
	return _running;
}

void GameState::update() 
{
	if (NetworkManager::instance()->isServer()) {
		std::string msg;
		bool gameOver = false;

		if (!(timeLeft < 1 || !_running) && (GameState::instance()->game_mode != ELIMINATION))
		{
			pt::ptime now = pt::second_clock::local_time();
			pt::time_duration diff = now - _start;

			int before = timeLeft;
			timeLeft = originalTime - diff.total_seconds();

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
			msg = "This game is over";
			gameOver = true;
		}
		else if(_running && GameState::instance()->game_mode == ELIMINATION)
		{
			if(num_player_left_elimination <= 1)
			{
				//display end game message
				//hang for some time
				//send message to end the game to return to main menu
				msg = "This game is over 2";
				gameOver = true;
			}
		}

		if (gameOver) {
			NetworkManager::instance()->sendGameOverPacket(msg);
			GameState::instance()->stop();
			LOG("SHOWING GAME OVER MESSAGE "<<msg);
		}
	}
}

void GameState::clear_old_games(){
	pt::ptime now=pt::second_clock::local_time();
	auto i=games.begin();
	while(i!=games.end()){
		if((now-(*i).second.second).total_seconds()>4){
			games.erase(i++);
		}else{
			++i;
		}
	}
}

bool GameState::nameIsTaken(char* name) {
	for(std::map<int,bool>::iterator iter = GameState::instance()->playerConnections.begin(); iter != GameState::instance()->playerConnections.end(); ++iter)
    {
        int i = iter->first;
		if (strcmp(name, getPlayerName(i).c_str()) == 0) {
			return true;
		}
	}
	return false;
}

void GameState::setPlayerName(int id, std::string name) {
	playerConnections[id] = true;
	playerNames[id] = name;
}

std::string GameState::getPlayerName(int id) {
	return playerNames[id];
}

void GameState::removePlayer(int id) {
	playerConnections.erase(id);
	playerNames.erase(id);
	if (players[id]) delete players[id];
	players[id] = NULL;
}

void GameState::stop() {
	_running = false;
}
