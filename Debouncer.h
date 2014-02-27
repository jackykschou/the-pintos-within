#ifndef __Debouncer_h_
#define __Debouncer_h_

#include "common.h"

class Debouncer {
  public:
	Debouncer(int waitMilliseconds);
	void run(void (*lambda)());

  protected:
  	int _waitMilliseconds;
  	boost::posix_time::ptime *_lastRun;
};

#endif
