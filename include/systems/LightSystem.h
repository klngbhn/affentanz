#pragma once

#include <typeinfo>

#include "System.h"

class LightSystem: public System, public IEventListener
{
public:
	void start();
	void update(float deltaTime);
	void stop();

	std::string getName() { return "LightSystemListener"; }
	bool handleEvent(const GameEvent& event);
private:
	void initLightComponent(std::shared_ptr<Component> component);
	void removeLightComponent(std::shared_ptr<Component> component);
};
