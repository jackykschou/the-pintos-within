#ifndef __GameState_h_
#define __GameState_h_

#include "common.h"
#include "Singleton.h"
#include "AudioManager.h"

class PlayerSpawner;
class PlayerCharacter;

#define DEFAULT_CLOCK 60 * 2 // 2 minutes

class PlayerCharacter;

class GameState : public Singleton<GameState>
{
  public:
    GameState();

  	int score;
  	int timeLeft;
  	void reset();
  	void update();
	  void start();
	  bool isRunning();

    PlayerCharacter* player;
    PlayerSpawner* spawner;
    PlayerCharacter* players[MAX_PLAYER];

    void broadcastHeartbeat();

  private:
  	boost::posix_time::ptime _start;
  	bool _running;
};

#endif // __GameState_h_
