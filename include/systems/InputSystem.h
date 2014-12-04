#pragma once

#include <typeinfo>

#include "OISEvents.h"
#include "OISInputManager.h"
#include "OISMouse.h"
#include "OISKeyboard.h"
#include "OISJoyStick.h"

#if OGRE_PLATFORM == OGRE_PLATFORM_APPLE_IOS
#else
#include "MyGUI.h"
#include "MyGUI_OgrePlatform.h"
#endif

#include "OgreStringConverter.h"

#include "System.h"

class InputSystem: public System, public OIS::MouseListener, public OIS::KeyListener, public OIS::JoyStickListener
{
public:
	void start();
	void update(float deltaTime);
	void stop();

	void setWindowExtents(int width, int height);

	// MouseListener
	bool mouseMoved(const OIS::MouseEvent &event);
	bool mousePressed(const OIS::MouseEvent &event, OIS::MouseButtonID);
	bool mouseReleased(const OIS::MouseEvent &event, OIS::MouseButtonID);

	// KeyListener
	bool keyPressed(const OIS::KeyEvent &event);
	bool keyReleased(const OIS::KeyEvent &event);

	// JoyStickListener
	bool buttonPressed(const OIS::JoyStickEvent &event, int index);
	bool buttonReleased(const OIS::JoyStickEvent &event, int index);
	bool axisMoved(const OIS::JoyStickEvent &event, int index);
	bool povMoved(const OIS::JoyStickEvent &event, int index);
private:
	void initInputComponent(std::shared_ptr<Component> component);

	unsigned long hWnd;
	OIS::InputManager *oiSystem;
	OIS::Mouse *mouse;
	OIS::Keyboard *keyboard;
};
