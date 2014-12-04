#pragma once

#include <typeinfo>

#include "System.h"

class MeshSystem: public System, public IEventListener
{
public:
	void start();
	void update(float deltaTime);
	void stop();

	std::string getName() { return "MeshSystemListener"; }
	bool handleEvent(const GameEvent& event);
private:
	void initMeshComponent(std::shared_ptr<Component> component);
	void removeMeshComponent(std::shared_ptr<Component> component);
};
