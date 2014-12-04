#pragma once

#include "Ogre.h"

#include "ComponentManager.h"
#include "IEventManager.h"
#include "Log.h"
#include "ConfigLoader.h"

/*
 * A system contains the logic for a specific type of component
 * and has access to the ComponentManager, the SceneManager,
 * the RenderWindow and the Config.
 * Start, update and stop methods should be overwritten in child
 * classes and will be called in the beginning (start) or ending
 * (stop) of a scene or each frame (update).
 */
class System
{
public:
	System() {}
	virtual ~System() {}
	void setComponentManager(std::shared_ptr<ComponentManager> componentMgr) { this->componentMgr = componentMgr; }
	void setSceneManager(Ogre::SceneManager* sceneMgr) { this->sceneMgr = sceneMgr; }
	void setRenderWindow(Ogre::RenderWindow* window) { this->window = window; }
	Ogre::RenderWindow* getRenderWindow() { return this->window; }
	void setConfig(std::shared_ptr<ConfigLoader> config) { this->config = config; }
	virtual void start() {}
	virtual void update(float deltaTime) {}
	virtual void stop() {}
protected:
	std::shared_ptr<ComponentManager> componentMgr;
	Ogre::SceneManager* sceneMgr;
	Ogre::RenderWindow* window;
	std::shared_ptr<ConfigLoader> config;
};
