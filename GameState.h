#ifndef __GameState_h_
#define __GameState_h_

#include "common.h"
#include "Singleton.h"

#define DEFAULT_CLOCK 2 * 60 // 2 minutes

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
