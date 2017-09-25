#ifndef __PERF_TIMER_H__
#define __PERF_TIMER_H__

#include "../Globals.h"

class PerfTimer
{
public:
	PerfTimer();
	virtual ~PerfTimer();

	void Start();
	void Stop();

	double ReadMs()const;
	uint64 ReadTicks()const;

private:
	bool running;
	uint64 startedAt;
	uint64 stopedAt;
	static uint64 frequency;
};

#endif // !__PERF_TIMER_H__