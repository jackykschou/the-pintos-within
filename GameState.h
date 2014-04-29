#ifndef __GameState_h_
#define __GameState_h_

#include "common.h"
#include "Singleton.h"
#include "AudioManager.h"

class PlayerSpawner;
class PlayerCharacter;
class WeaponSpawner;

#define DEFAULT_CLOCK 60 * 2 // 2 minutes

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

  	int score;
  	int timeLeft;
  	void reset();
  	void update();
	  void start();
	  bool isRunning();

    PlayerCharacter* player;
    PlayerSpawner* spawner;
    WeaponSpawner* weapon_spawner;
    PlayerCharacter* players[MAX_PLAYER];

    void broadcastHeartbeat();

  private:
  	boost::posix_time::ptime _start;
  	bool _running;
};

#endif // __GameState_h_
