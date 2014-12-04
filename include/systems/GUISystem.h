#pragma once

#include <typeinfo>

#include "MyGUI.h"
#include "MyGUI_OgrePlatform.h"

#include "System.h"
#include "DebugView.h"

class GUISystem: public System, public IEventListener
{
public:
	void start();
	void update(float deltaTime);
	void stop();

	std::string getName() { return "GUISystemListener"; }
	bool handleEvent(const GameEvent& event);
private:
	void initGUIComponent(std::shared_ptr<Component> component);
	void removeGUIComponent(std::shared_ptr<Component> component);
	MyGUI::MapString getStatistics();

	std::shared_ptr<DebugView> debugView;
};
