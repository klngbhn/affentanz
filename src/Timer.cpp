#include "Timer.h"

Timer::Timer(unsigned long timeSinceStart)
{
	currentTime  = 0.0f;
	lastTime = timeSinceStart / 1000.0f;
	elapsed  = 0.0f;
}

Timer::~Timer()
{

}

void Timer::Update(unsigned long timeSinceStart)
{
	// calculate elapsed time since last frame
	currentTime = timeSinceStart / 1000.0f;

	elapsed = currentTime - lastTime;
	lastTime = currentTime;
}
