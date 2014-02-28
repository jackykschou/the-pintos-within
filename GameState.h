#ifndef __GameState_h_
#define __GameState_h_

#include "common.h"
#include "Singleton.h"
#include "GUIManager.h"

#define DEFAULT_CLOCK 10 // 2 minutes

class GameState : public Singleton<GameState>{
  public:
  	int score;
  	int timeLeft;
  	void reset();
  	void update();
  private:
  	boost::posix_time::ptime _start;
};

#endif // __GameState_h_
