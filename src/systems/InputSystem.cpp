#include "InputSystem.h"

void InputSystem::start()
{
	window->getCustomAttribute("WINDOW", &hWnd);

	OIS::ParamList pl;
	pl.insert(OIS::ParamList::value_type("WINDOW", Ogre::StringConverter::toString(hWnd)));

	oiSystem = OIS::InputManager::createInputSystem( pl );
	mouse = static_cast<OIS::Mouse*>(oiSystem->createInputObject( OIS::OISMouse, true ));
	keyboard = static_cast<OIS::Keyboard*>(oiSystem->createInputObject( OIS::OISKeyboard, true));
	mouse->setEventCallback(this);
	keyboard->setEventCallback(this);

	mouse->getMouseState().height = window->getHeight();
	mouse->getMouseState().width  = window->getWidth();

	LOG_DEBUG("Input system started");
}

void InputSystem::update(float deltaTime)
{
	mouse->capture();
	keyboard->capture();
}

void InputSystem::stop()
{
	if (mouse)
		oiSystem->destroyInputObject(mouse);
	if (keyboard)
		oiSystem->destroyInputObject(keyboard);

	OIS::InputManager::destroyInputSystem(oiSystem);

	LOG_DEBUG("Input system stopped");
}

void  InputSystem::setWindowExtents(int width, int height)
{
    const OIS::MouseState &ms = mouse->getMouseState();
    ms.width = width;
    ms.height = height;
}

// MouseListener
bool InputSystem::mouseMoved(const OIS::MouseEvent &event)
{
	if (MyGUI::InputManager::getInstance().injectMouseMove(event.state.X.abs, event.state.Y.abs, event.state.Z.abs))
		return true;

	std::vector<std::shared_ptr<Component>> components = componentMgr->getComponentsByType(&typeid(InputComponent));

	for (std::shared_ptr<Component> component : components)
	{
		dynamic_cast<InputComponent*>(component.get())->mouseMoved(event);
	}

	return true;
}

bool InputSystem::mousePressed(const OIS::MouseEvent &event, OIS::MouseButtonID button)
{
	if (MyGUI::InputManager::getInstance().injectMousePress(event.state.X.abs, event.state.Y.abs, MyGUI::MouseButton::Enum(button)))
		return true;

	std::vector<std::shared_ptr<Component>> components = componentMgr->getComponentsByType(&typeid(InputComponent));

	for (std::shared_ptr<Component> component : components)
	{
		dynamic_cast<InputComponent*>(component.get())->mousePressed(event, button);
	}

	return true;
}

bool InputSystem::mouseReleased(const OIS::MouseEvent &event, OIS::MouseButtonID button)
{
	if (MyGUI::InputManager::getInstance().injectMouseRelease(event.state.X.abs, event.state.Y.abs, MyGUI::MouseButton::Enum(button)))
		return true;

	std::vector<std::shared_ptr<Component>> components = componentMgr->getComponentsByType(&typeid(InputComponent));

	for (std::shared_ptr<Component> component : components)
	{
		dynamic_cast<InputComponent*>(component.get())->mouseReleased(event, button);
	}

	return true;
}

// KeyListener
bool InputSystem::keyPressed(const OIS::KeyEvent &event)
{
	if (MyGUI::InputManager::getInstance().injectKeyPress(MyGUI::KeyCode::Enum(event.key), event.text))
		return true;

	std::vector<std::shared_ptr<Component>> components = componentMgr->getComponentsByType(&typeid(InputComponent));

	for (std::shared_ptr<Component> component : components)
	{
		dynamic_cast<InputComponent*>(component.get())->keyPressed(event);
	}

	return true;
}

bool InputSystem::keyReleased(const OIS::KeyEvent &event)
{
	if (MyGUI::InputManager::getInstance().injectKeyRelease(MyGUI::KeyCode::Enum(event.key)))
		return true;

	if(event.key == OIS::KC_SYSRQ)
	{
		GameEventPtr event(new SystemEvent("TakeScreenshot"));
		safeQueueEvent(event);
		return true;
	}

	std::vector<std::shared_ptr<Component>> components = componentMgr->getComponentsByType(&typeid(InputComponent));

	for (std::shared_ptr<Component> component : components)
	{
		dynamic_cast<InputComponent*>(component.get())->keyReleased(event);
	}

	return true;
}

// JoyStickListener
bool InputSystem::buttonPressed(const OIS::JoyStickEvent &event, int index) {
    return true;
}

bool InputSystem::buttonReleased(const OIS::JoyStickEvent &event, int index) {
    return true;
}

bool InputSystem::axisMoved(const OIS::JoyStickEvent &event, int index) {
    return true;
}

bool InputSystem::povMoved(const OIS::JoyStickEvent &event, int index) {
    return true;
}
