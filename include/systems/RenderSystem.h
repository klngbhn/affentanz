#pragma once

#include <typeinfo>

#include "System.h"
#include "Fader.h"

class RenderSystem: public System, public IEventListener
{
public:
	void start();
	void update(float deltaTime);
	void stop();

	std::string getName() { return "RenderSystemListener"; }
	bool handleEvent(const GameEvent& event);
private:
	void initRenderComponent(std::shared_ptr<Component> component);
	void removeRenderComponent(std::shared_ptr<Component> component);
	std::shared_ptr<Fader> fader;
};
