#pragma once

#include <typeinfo>

#include <lua.hpp>

#include "System.h"

class ScriptingSystem: public System, public IEventListener
{
public:
	void start();
	void update(float deltaTime);
	void stop();

	std::string getName() { return "ScriptingSystemListener"; }
	bool handleEvent(const GameEvent& event);
	void insertEventListener(EventType eventType, std::string functionName);
	void delEventListener(EventType eventType);
	std::shared_ptr<ComponentManager> getComponentMgr() { return componentMgr; }
private:
	void loadScripts();
	lua_State *luaState;
	std::vector<std::string> systems;
	std::map<EventType, std::vector<std::string>> eventListeners;
};
