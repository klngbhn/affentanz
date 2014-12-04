#include "TouchInputSystem.h"

void TouchInputSystem::start()
{
//	touch = new AndroidMultiTouch(renderWindowName);
	keyboard = new AndroidKeyboard();

	LOG_DEBUG("Touch input system started");
}

void TouchInputSystem::update(float deltaTime)
{

}

void TouchInputSystem::stop()
{
	SAFE_DELETE(keyboard);
	LOG_DEBUG("Touch input system stopped");
}

void TouchInputSystem::injectKeyEvent(int action, int32_t keyCode)
{
    if(keyCode == AKEYCODE_BACK)
    {
        //OIS::KeyEvent evt(this, OIS::KC_ESCAPE, 0);
        if(action == 0)
        {
            //mBrowser->keyPressed(evt);
        }
        else
        {
            //mBrowser->keyReleased(evt);
        }
    }
}

void TouchInputSystem::injectTouchEvent(int action, float x, float y, int pointerId)
{
    OIS::MultiTouchState &state = getMultiTouchState(pointerId);

    switch(action)
    {
        case 0:
            state.touchType = OIS::MT_Pressed;
            break;
        case 1:
            state.touchType = OIS::MT_Released;
            break;
        case 2:
            state.touchType = OIS::MT_Moved;
            break;
        case 3:
            state.touchType = OIS::MT_Cancelled;
            break;
        default:
            state.touchType = OIS::MT_None;
    }

    if(state.touchType != OIS::MT_None)
    {
        int last = state.X.abs;
        state.X.abs =  (int)x;
        state.X.rel = state.X.abs - last;

        last = state.Y.abs;
        state.Y.abs = (int)y;
        state.Y.rel = state.Y.abs - last;

        state.Z.abs = 0;
        state.Z.rel = 0;

        OIS::MultiTouchEvent event(this, state);

        switch(state.touchType)
        {
            case OIS::MT_Pressed:
				{
					MyGUI::InputManager::getInstance().injectMouseMove(event.state.X.abs, event.state.Y.abs, event.state.Z.abs);
					if (MyGUI::InputManager::getInstance().injectMousePress(event.state.X.abs, event.state.Y.abs, MyGUI::MouseButton::Enum(OIS::MouseButtonID::MB_Left)))
						break;

					std::vector<std::shared_ptr<Component>> components = componentMgr->getComponentsByType(&typeid(InputComponent));

					for (std::shared_ptr<Component> component : components)
					{
						dynamic_cast<InputComponent*>(component.get())->touchPressed(event);
					}
				}
            	break;
            case OIS::MT_Released:
                {
					if (MyGUI::InputManager::getInstance().injectMouseRelease(event.state.X.abs, event.state.Y.abs, MyGUI::MouseButton::Enum(OIS::MouseButtonID::MB_Left)))
						break;

                	std::vector<std::shared_ptr<Component>> components = componentMgr->getComponentsByType(&typeid(InputComponent));

                	for (std::shared_ptr<Component> component : components)
                	{
                		dynamic_cast<InputComponent*>(component.get())->touchReleased(event);
                	}
				}
                break;
            case OIS::MT_Moved:
                {
                	MyGUI::InputManager::getInstance().injectMouseMove(event.state.X.abs, event.state.Y.abs, event.state.Z.abs);
					std::vector<std::shared_ptr<Component>> components = componentMgr->getComponentsByType(&typeid(InputComponent));

					for (std::shared_ptr<Component> component : components)
					{
						dynamic_cast<InputComponent*>(component.get())->touchMoved(event);
					}
				}
                break;
            case OIS::MT_Cancelled:
                break;
            default:
                break;
        }
    }
}
