#include "Debouncer.h"

Debouncer::Debouncer(int waitMilliseconds)
{
	_waitMilliseconds = waitMilliseconds;
	_lastRun = NULL;
}

bool Debouncer::run(void (*lambda)()) 
{
	boost::posix_time::ptime now;
	now = boost::posix_time::second_clock::local_time();
	if (!_lastRun || (now - *_lastRun).total_milliseconds() > _waitMilliseconds) 
	{
		// fucking do it.
		if (_lastRun) delete _lastRun;
		_lastRun = new boost::posix_time::ptime(now);
		if(lambda)
			lambda();
		
		return true;
	}
	return false;
}

void Debouncer::updateTimer(float time)
{
	_waitMilliseconds = time;
}
