#pragma once

#include <typeinfo>

//#if OGRE_PLATFORM == OGRE_PLATFORM_APPLE_IOS
//#else
#include "MyGUI.h"
#include "MyGUI_OgrePlatform.h"
//#endif

#include "System.h"

class CameraSystem: public System, public IEventListener
{
public:
	void start();
	void update(float deltaTime);
	void stop();

	std::string getName() { return "CameraSystemListener"; }
	bool handleEvent(const GameEvent& event);
private:
	void initCameraComponent(std::shared_ptr<Component> component);
	void removeCameraComponent(std::shared_ptr<Component> component);
};
