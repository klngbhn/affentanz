#pragma once

#include <typeinfo>

#include "System.h"

class AnimationSystem: public System, public IEventListener
{
public:
	void start();
	void update(float deltaTime);
	void stop();

	std::string getName() { return "AnimationSystemListener"; }
	bool handleEvent(const GameEvent& event);
private:
	void initAnimationComponent(std::shared_ptr<Component> component);
	void removeAnimationComponent(std::shared_ptr<Component> component);
	void changeAnimation(int objectID, std::string animationName);
};
