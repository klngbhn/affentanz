 #pragma once

#include <typeinfo>

#include "System.h"

class TransformSystem: public System, public IEventListener
{
public:
	void start();
	void update(float deltaTime);
	void stop();

	std::string getName() { return "TransformSystemListener"; }
	bool handleEvent(const GameEvent& event);
private:
	void removeTransformComponent(std::shared_ptr<Component> component);
};
