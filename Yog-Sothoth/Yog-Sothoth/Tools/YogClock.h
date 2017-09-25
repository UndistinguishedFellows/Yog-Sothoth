#ifndef __CLOCK_H__
#define __CLOCK_H__

#include "../Globals.h"

class PerfTimer;

class YogClock
{
public:
	YogClock();
	virtual ~YogClock();

	void OnPrepareUpdate();
	void OnFinishUpdate();

	void OnSceneLoaded();

	void Pause();
	void UnPause();
	void Stop();
	void Play();

	//Some getters---------------
	float GetDT()const;
	uint GetRealFrameCount()const;
	uint GetFPS()const;
	uint GetLastFPS()const;

	float GetTimeSinceStart()const;
	float GetTimeSinceSceneLoaded()const;

	float GetGameDT()const;
	uint GetGameFrameCounter()const;
	float GetGameTimeSinceLevelLoaded()const;

	float GetScale()const;

	float LastFrameMs()const;

	void SetScale(float scl);


private:
	//Real -------------------------
	PerfTimer* msTimer = nullptr;
	PerfTimer* fpsTimer = nullptr;
	float realDt = 0.f;
	uint realFrameCount = 0;
	uint fpsCounter = 0;
	uint lastFps = 0;

	float timeSinceAppStart = 0.f; //Time in seconds since start up
	float timeSinceLevelLoaded = 0.f;

	//Game -------------------------
	PerfTimer* msGameTimer = nullptr;
	float gameDt = 0.f;
	uint gameFrameCount = 0;
	float gameTimeSinceLevelLoaded = 0.f;

	//-----------------------------------

	float scale = 1.0f;

	float lastFrameMs = 0;
	float maximumDT = 1.0f;
};

#endif // !__GG_CLOCK_H__