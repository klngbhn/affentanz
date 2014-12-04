#pragma once

#include <typeinfo>

#include <android_native_app_glue.h>

#include "OIS.h"

#include "MyGUI.h"
#include "MyGUI_OgrePlatform.h"

#include "OgreStringConverter.h"

#include "System.h"

class AndroidMultiTouch : public OIS::MultiTouch
{
public:
    AndroidMultiTouch(std::string windowName):OIS::MultiTouch("Ogre", false, 0, 0), renderWindowName(windowName){}

    virtual void setBuffered(bool buffered) { }
    virtual void capture() { }
    virtual OIS::Interface* queryInterface(OIS::Interface::IType type) { return 0; }
    virtual void _initialize() { }

    OIS::MultiTouchState &getMultiTouchState(int i)
    {
        while(i >= mStates.size())
        {
            Ogre::RenderWindow* pRenderWnd = static_cast<Ogre::RenderWindow*>(Ogre::Root::getSingleton().getRenderTarget(renderWindowName));
            if(pRenderWnd)
            {
                OIS::MultiTouchState state;
                state.width = pRenderWnd->getWidth();
                state.height = pRenderWnd->getHeight();
                mStates.push_back(state);
            }
        }
        return mStates[i];
    }

private:
	std::string renderWindowName;
};

class AndroidKeyboard : public OIS::Keyboard
{
public:
    AndroidKeyboard():OIS::Keyboard("Ogre", false, 1, 0) { }

    virtual void setBuffered(bool buffered){}
    virtual void capture(){}
    virtual OIS::Interface* queryInterface(OIS::Interface::IType type) {return 0;}
    virtual void _initialize(){}
    virtual bool isKeyDown( OIS::KeyCode key ) const { return false; }

    virtual const std::string& getAsString( OIS::KeyCode kc )
    {
        static std::string defstr = "";
        return defstr;
    }

    virtual void copyKeyStates( char keys[256] ) const{ }
};

class TouchInputSystem: public System, public OIS::MultiTouch
{
public:
	TouchInputSystem():OIS::MultiTouch("Ogre", false, 0, 0) { }

    virtual void setBuffered(bool buffered){}
    virtual void capture(){}
    virtual OIS::Interface* queryInterface(OIS::Interface::IType type) {return 0;}
    virtual void _initialize(){}

    OIS::MultiTouchState &getMultiTouchState(int i)
	{
		while(i >= mStates.size())
		{
			if(window)
			{
				OIS::MultiTouchState state;
				state.width = window->getWidth();
				state.height = window->getHeight();
				mStates.push_back(state);
			}
		}
		return mStates[i];
	}

	void start();
	void update(float deltaTime);
	void stop();

	void injectKeyEvent(int action, int32_t keyCode);
	void injectTouchEvent(int action, float x, float y, int pointerId = 0);

private:
//	AndroidMultiTouch* touch;
	AndroidKeyboard* keyboard;
};
