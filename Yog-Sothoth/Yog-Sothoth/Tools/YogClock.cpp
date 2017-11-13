#include "PerfTimer.h"
#include "YogClock.h"


YogClock::YogClock()
{
	msTimer = new PerfTimer();
	msGameTimer = new PerfTimer();
	fpsTimer = new PerfTimer();
}

YogClock::~YogClock()
{
	RELEASE(msTimer);
	RELEASE(msGameTimer);
	RELEASE(fpsTimer);
}

/**
*	- OnPrepareUpdate: Calculate all attributes at the begining of each frame
*		- Add the elapsed time.
*		- Recal real dt.
*		- Add one frame to counter.
*		- If app state is PLAY do the same with the game timer.
*/
void YogClock::OnPrepareUpdate(game_status gameStatus)
{
	//1. Add time
	timeSinceAppStart += realDt;
	timeSinceLevelLoaded += realDt;
	//2. Calc dt
	realDt = (float)(msTimer->ReadMs() / 1000.0f);
	if (realDt > maximumDT) realDt = 1 / 30.0f;
	msTimer->Start();
	//3. Add a frame
	++realFrameCount;

	if (gameStatus == game_status::PLAY)
	{
		gameTimeSinceLevelLoaded += gameDt;

		gameDt = ((float)(msGameTimer->ReadMs() / 1000.0f)) * scale;
		msGameTimer->Start();

		++gameFrameCount;
	}
}

/**
*	- OnFinishUpdate: Calculate all attributes at the ending of the frame.
*		- Add one to fps counter.
*		- Check if one second has passed to calculate fps.
*		- Calculate last frame ms.
*/
void YogClock::OnFinishUpdate()
{
	++fpsCounter;

	if (fpsTimer->ReadMs() > 1000.0f)
	{
		lastFps = fpsCounter;
		fpsCounter = 0;
		fpsTimer->Start();
	}

	lastFrameMs = msTimer->ReadMs();
}

void YogClock::OnSceneLoaded()
{
	timeSinceLevelLoaded = 0.0;
}

void YogClock::Pause()
{
	msGameTimer->Stop();
}

void YogClock::UnPause()
{
	msGameTimer->Start();
}

void YogClock::Stop()
{
	msGameTimer->Stop();
	msGameTimer->Reset();
	gameFrameCount = 0;
	gameDt = 0.0f;
	gameTimeSinceLevelLoaded = 0.0f;
}

void YogClock::Play()
{
	msGameTimer->Start();
}

/**
*	- GetDT: Return real dt.
*/
float YogClock::GetDT() const
{
	return realDt;
}

/**
*	- GetRealFrameCount: Return the real frames passed.
*/
uint YogClock::GetRealFrameCount() const
{
	return realFrameCount;
}

/**
*	- GetFPS: Return the fps counter.
*/
uint YogClock::GetFPS() const
{
	return fpsCounter;
}

/**
*	- GetLastFPS: Return the last frame fps counter.
*/
uint YogClock::GetLastFPS() const
{
	return lastFps;
}

/**
*	- GetTimeSinceStart: Return the time in seconds since app started.
*/
float YogClock::GetTimeSinceStart() const
{
	return timeSinceAppStart;
}

/**
*	- GetTimeSinceSceneLoaded: Return the time in seconds since scene was loaded.
*/
float YogClock::GetTimeSinceSceneLoaded() const
{
	return timeSinceLevelLoaded;
}

/**
*	- GetGameDT: Return the game dt.
*/
float YogClock::GetGameDT() const
{
	return gameDt;
}

/**
*	- GetGameFrameCounter: Return the game frames passed.
*/
uint YogClock::GetGameFrameCounter() const
{
	return gameFrameCount;
}

/**
*	- GetTimeSinceSceneLoaded: Return the time in seconds since scene was loaded in game scale.
*/
float YogClock::GetGameTimeSinceLevelLoaded() const
{
	return gameTimeSinceLevelLoaded;
}

/**
*	- GetScale: Return the game time scale.
*/
float YogClock::GetScale() const
{
	return scale;
}

/**
*	- LastFrameMs: Return the last frame ms.
*/
float YogClock::LastFrameMs() const
{
	return lastFrameMs;
}

/**
*	- SetScale: Set the game time scale.
*/
void YogClock::SetScale(float scl)
{
	if (scl >= 0.0f)
		scale = scl;
	else
		scale = 0.0f;
}