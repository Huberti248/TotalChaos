#pragma once
#include <SDL.h>
#include <string>

class Clock {
public:
	Uint64 start;

	Clock();
	~Clock();
	
	float getElapsedTime();

	float restart();

	static Clock* GetGlobalClock();

	static float GetDeltaTime();

private:
	static Clock globalClock;
	static float deltaTime;
};