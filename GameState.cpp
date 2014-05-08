#include "GameState.h"
#include "PlayerSpawner.h"
#include "PlayerCharacter.h"
#include "NetworkManager.h"
#include "SceneManager.h"
#include "Debouncer.h"

namespace pt = boost::posix_time;

GameState::GameState()
{
	score = 0;
	_start = pt::second_clock::local_time();
	_running = false;
}

void GameState::reset() 
{
	_gameOver = false;
	end_game_debouncer = NULL;
	score = 0;
	timeLeft = originalTime;
	_start = pt::second_clock::local_time();
	_running = false;
	players.clear();

	AudioManager::instance()->stopMusic();
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

	SceneManager::instance()->changeCurrentScene(GameState::instance()->current_map);

	carrying_pinto_seed = false;

	AudioManager::instance()->startMusic();

	spawner->startGame();
}

bool GameState::isRunning() {
	return _running;
}

void GameState::update() 
{
	clear_old_games();
	if (NetworkManager::instance()->isServer() && !_gameOver) 
	{

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
			_gameOver = true;
			NetworkManager::instance()->sendGameOverPacket();
		}
		else if(_running && GameState::instance()->game_mode == ELIMINATION)
		{
			if(num_player_left_elimination <= 1)
			{
				_gameOver = true;
				NetworkManager::instance()->sendGameOverPacket();
				gameOver = true;
			}
		}
	}

	if(end_game_debouncer != NULL && end_game_debouncer->run(NULL) && _running)
	{
		GuiManager::instance()->HideMessage();

		player_pinto_seeds.clear();
	    playerNames.clear();
	    playerConnections.clear();
	    players.clear();

	    GameState::instance()->current_state = MAIN_MENU;

		if(SceneManager::instance()->current_scene != NULL)
		{
			delete (SceneManager::instance()->current_scene);
			SceneManager::instance()->current_scene = NULL;
		}

		GuiManager::instance()->Reinitialize();

		delete end_game_debouncer;
		end_game_debouncer = NULL;

		reset();
	}
}

void GameState::clear_old_games(){

	pt::ptime now=pt::second_clock::local_time();
	auto i=games.begin();
	while(i!=games.end())
	{
		if((now-(*i).second.second).total_seconds() > 4 ){
			games.erase(i++);
		}else{
			++i;
		}
	}

	GameState::instance()->current_state = MAIN_MENU;

	if(SceneManager::instance()->current_scene != NULL)
	{
		delete (SceneManager::instance()->current_scene);
		SceneManager::instance()->current_scene = NULL;
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
	playerScores[id] = 0;
}

std::string GameState::getPlayerName(int id) {
	return playerNames[id];
}

void GameState::removePlayer(int id) {
	playerConnections.erase(id);
	playerNames.erase(id);
	if (players[id]) {
		if (((GameObject*)players[id])->scene) {
			((GameObject*)players[id])->scene->removeGameObject((GameObject*)players[id]);
		}
		delete players[id];
	}
	players[id] = NULL;
}


void GameState::stop(std::string message) 
{
	end_game_debouncer = new Debouncer(5.0 * 1000);
	end_game_debouncer->run();
	GuiManager::instance()->DisplayMessage(message);
	LOG("STOPPING MESSAGE: "<<message);
}
