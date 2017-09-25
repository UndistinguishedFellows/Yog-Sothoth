#include "PerfTimer.h"
#include <SDL_timer.h>

uint64 PerfTimer::frequency = 0;

PerfTimer::PerfTimer()
{
	if (frequency == 0)
		frequency = SDL_GetPerformanceFrequency();

	Start();
}
PerfTimer::~PerfTimer()
{
}

void PerfTimer::Start()
{
	running = true;
	startedAt = SDL_GetPerformanceCounter();
}

void PerfTimer::Stop()
{
	running = false;
	stopedAt = SDL_GetPerformanceCounter();
}

double PerfTimer::ReadMs() const
{
	if (running)
		return 1000.0 * (double(SDL_GetPerformanceCounter() - startedAt) / double(frequency));
	else
		return 1000.0 * (stopedAt - startedAt);
}

uint64 PerfTimer::ReadTicks() const
{
	if (running)
		return SDL_GetPerformanceCounter() - startedAt;
	else
		return stopedAt - startedAt;
}