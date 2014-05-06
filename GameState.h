#ifndef __GameState_h_
#define __GameState_h_

#include <map>
#include "common.h"
#include "Singleton.h"
#include "AudioManager.h"

class PlayerSpawner;
class PlayerCharacter;
class WeaponSpawner;

#define DEFAULT_CLOCK 2 // 2 minutes

class PlayerCharacter;

class GameState : public Singleton<GameState>
{
  public:
    GameState();

    uint32_t team_id;

    uint32_t current_map;
    uint32_t game_mode;
    uint32_t team_mode;
    uint32_t current_state;
    int num_player;
    int current_max_num_player_map;
    bool carrying_pinto_seed;

  	int score;
  	int timeLeft;
    int originalTime;
    int num_player_left_elimination;
  	void reset();
  	void update();
	  void start();
	  bool isRunning();
    bool nameIsTaken(char* name);

    PlayerCharacter* player;
    PlayerSpawner* spawner;
    WeaponSpawner* weapon_spawner;

    std::map<int, bool> player_pinto_seeds;
    std::map<int, std::string> playerNames;
    std::map<int, bool> playerConnections;
    std::map<int, PlayerCharacter*> players;

    std::map<std::string,std::pair<std::string,boost::posix_time::ptime>> games;

    void broadcastHeartbeat();

    void setPlayerName(int id, std::string name);
    std::string getPlayerName(int id);
    void removePlayer(int id);
    void stop();

  private:

  	boost::posix_time::ptime _start;
  	bool _running;
  	void clear_old_games();
};

#endif // __GameState_h_
