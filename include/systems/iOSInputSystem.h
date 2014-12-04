#pragma once

#include <typeinfo>

#include "OIS.h"

#include "System.h"

#ifdef __OBJC__
#import "InputGestureView.h"
@class InputGestureView;
@class UIWindow;
#else
typedef struct objc_object InputGestureView;
typedef struct objc_object UIWindow;
#endif

class iOSInputSystem: public System, public OIS::MultiTouch
{
public:
    iOSInputSystem():OIS::MultiTouch("Ogre", false, 0, 0),gestureView(0) { }
    virtual ~iOSInputSystem();
    
    virtual void setBuffered(bool buffered){}
    virtual void capture(){}
    virtual OIS::Interface* queryInterface(OIS::Interface::IType type) {return 0;}
    virtual void _initialize(){}

	void start();
	void update(float deltaTime);
	void stop();

    void motionBegan() {}
    void motionEnded() {}
    void motionCancelled() {}
    void touchBegan(float x, float y);
    void touchMoved(float x, float y);
    void touchEnded(float x, float y);

private:
    InputGestureView* gestureView;
    UIWindow* mUIWindow;
};

