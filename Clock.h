#pragma once
#include <SDL.h>

struct Clock {
	Uint64 start = SDL_GetPerformanceCounter();
	
	float getElapsedTime()
	{
		Uint64 stop = SDL_GetPerformanceCounter();
		float secondsElapsed = (stop - start) / (float)SDL_GetPerformanceFrequency();
		return secondsElapsed * 1000;
	}

	float restart()
	{
		Uint64 stop = SDL_GetPerformanceCounter();
		float secondsElapsed = (stop - start) / (float)SDL_GetPerformanceFrequency();
		start = SDL_GetPerformanceCounter();
		return secondsElapsed * 1000;
	}
};