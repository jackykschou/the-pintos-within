#ifndef __GameState_h_
#define __GameState_h_

#include "common.h"
#include "Singleton.h"
#include "GUIManager.h"
#include "AudioManager.h"
#include "NetworkManager.h"

class PlayerSpawner;

#define DEFAULT_CLOCK 60 * 2 // 2 minutes

class GameState : public Singleton<GameState>{
  public:
  	int score;
  	int timeLeft;
  	void reset();
  	void update();
	  void start();
	  bool isRunning();

    PlayerSpawner *spawner;


  private:
  	boost::posix_time::ptime _start;
  	bool _running;
};

#endif // __GameState_h_
