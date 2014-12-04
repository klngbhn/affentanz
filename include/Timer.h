#pragma once

/*
 * This class represents a clock for calculating delta time.
 * It uses the ogre timer class for input.
 */
class Timer
{
public:
	Timer(unsigned long timeSinceStart);
	virtual ~Timer();
	void  Update(unsigned long timeSinceStart);
	float getElapsed () { return elapsed; }
private:
	float elapsed;  	// elapsed time since last frame
	float currentTime;  // current time
	float lastTime; 	// time of last frame
};
