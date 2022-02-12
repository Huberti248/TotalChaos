#include "Clock.h"

Clock Clock::globalClock;
float Clock::deltaTime;

Clock::Clock() {
	this->start = SDL_GetPerformanceCounter();
}

Clock::~Clock()
{
}

float Clock::getElapsedTime() {
	Uint64 stop = SDL_GetPerformanceCounter();
	float secondsElapsed = (stop - start) / (float)SDL_GetPerformanceFrequency();
	return secondsElapsed * 1000.0f;
}

float Clock::restart() {
	Uint64 stop = SDL_GetPerformanceCounter();
	float secondsElapsed = (stop - start) / (float)SDL_GetPerformanceFrequency();
	start = SDL_GetPerformanceCounter();
	float result = secondsElapsed * 1000.0f;
	int comparison = memcmp(this, &Clock::globalClock, sizeof(Clock));
	if (comparison == 0) {
		Clock::deltaTime = result;
	}
	return result;
}

Uint32 Clock::CalculateDelay(float deltaTime) {
	return (Uint32)MathUtils::Clamp(floor(DELAY_CAP - deltaTime), 0.0f, DELAY_CAP);
}

float Clock::GetDeltaTime() {
	return Clock::deltaTime;
}

Clock* Clock::GetGlobalClock() {
	return &Clock::globalClock;
}

