#ifndef __Debouncer_h_
#define __Debouncer_h_

#include "common.h"

class Debouncer 
{
  public:
	Debouncer(int waitMilliseconds);
	Debouncer(int waitMilliseconds, std::function<void()>);
	bool run(void (*lambda)());
	bool run();
	void updateTimer(float);

  protected:
	int _waitMilliseconds;
	std::function<void()> _lambda;
	boost::posix_time::ptime *_lastRun;
};

#endif
