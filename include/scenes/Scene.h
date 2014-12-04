#pragma once

#include <memory>

#include "Ogre.h"
#include "OgreOverlaySystem.h"

#include "IEventManager.h"
#include "GameEvent.h"
#include "ComponentManager.h"
#include "ConfigLoader.h"

#include "TransformSystem.h"
#include "CameraSystem.h"
#include "LightSystem.h"
#include "MeshSystem.h"
#include "RenderSystem.h"
#include "AnimationSystem.h"
#if OGRE_PLATFORM == OGRE_PLATFORM_APPLE_IOS
#include "iOSInputSystem.h"
#else
#include "GUISystem.h"
#endif
#include "InputSystem.h"
#include "PhysicsSystem.h"
#include "ScriptingSystem.h"
#if OGRE_PLATFORM == OGRE_PLATFORM_ANDROID
#include "TouchInputSystem.h"
#endif

/*
 * This is the basic scene class that contains SceneManager, RenderWindow, Config,
 * ComponentManager, all Systems and methods to start, stop, pause, resume and
 * update.
 */
class Scene
{
public:
	Scene() {};
	virtual ~Scene() {};

	void setRenderWindow(Ogre::RenderWindow* window) { this->window = window; }
	void setConfig(std::shared_ptr<ConfigLoader> config) { this->config = config; }
	void setOverlaySystem(Ogre::OverlaySystem* overlaySystem) { this->overlaySystem = overlaySystem; }
#if OGRE_PLATFORM == OGRE_PLATFORM_ANDROID
	std::shared_ptr<TouchInputSystem> getTouchInput() { return touchInput; }
#endif

	virtual bool pause(){ return true; }
    virtual void resume(){};
    virtual void enter() = 0;
    virtual void exit() = 0;
    virtual void update(float deltaTime) = 0;
protected:
	Ogre::SceneManager* sceneMgr;
	Ogre::RenderWindow* window;
	Ogre::OverlaySystem* overlaySystem;
	std::shared_ptr<ConfigLoader> config;
	std::shared_ptr<ComponentManager> componentMgr;
	std::vector<std::shared_ptr<System>> systems;

#if OGRE_PLATFORM == OGRE_PLATFORM_ANDROID
	std::shared_ptr<TouchInputSystem> touchInput;
#endif
};
