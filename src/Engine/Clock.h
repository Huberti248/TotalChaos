#pragma once
#include <SDL.h>
#include <string>
#include "../Auxiliars/MathUtils.h"

//This is to cap it to 60fps
#define DELAY_CAP 16.666f

class Clock {
public:
	Uint64 start;

	Clock();
	~Clock();
	
	float getElapsedTime();

	float restart();

	static Clock* GetGlobalClock();

	static float GetDeltaTime();

	static Uint32 CalculateDelay(float dt);

private:
	static Clock globalClock;
	static float deltaTime;
};