#include "iOSInputSystem.h"

iOSInputSystem::~iOSInputSystem()
{
    [gestureView release];
    [mUIWindow release];
}

void iOSInputSystem::start()
{
    //Get Ogre window
    mUIWindow = nil;
    window->getCustomAttribute("WINDOW", &mUIWindow);
    
    gestureView = [[InputGestureView alloc] initWithFrame:CGRectMake(0,0,window->getWidth(),window->getHeight())];
    //gestureView = [[InputGestureView alloc] init];
    gestureView.inputSystem = this;
    
    [mUIWindow addSubview:gestureView];
    
    [mUIWindow bringSubviewToFront:gestureView];
    [mUIWindow makeKeyAndVisible];
    [gestureView becomeFirstResponder];
    
	LOG_DEBUG("iOS input system started");
}

void iOSInputSystem::update(float deltaTime)
{

}

void iOSInputSystem::stop()
{
	LOG_DEBUG("iOS input system stopped");
}

void iOSInputSystem::touchBegan(float x, float y)
{
    OIS::MultiTouchState state;
    state.width = window->getWidth();
    state.height = window->getHeight();
    state.touchType = OIS::MT_Pressed;

    int last = state.X.abs;
    state.X.abs =  (int)x;
    state.X.rel = state.X.abs - last;
        
    last = state.Y.abs;
    state.Y.abs = (int)y;
    state.Y.rel = state.Y.abs - last;
        
    state.Z.abs = 0;
    state.Z.rel = 0;
        
    OIS::MultiTouchEvent event(this, state);
        
    std::vector<std::shared_ptr<Component>> components = componentMgr->getComponentsByType(&typeid(InputComponent));
    
    for (std::shared_ptr<Component> component : components)
    {
        dynamic_cast<InputComponent*>(component.get())->touchPressed(event);
    }
}

void iOSInputSystem::touchMoved(float x, float y)
{
    OIS::MultiTouchState state;
    state.width = window->getWidth();
    state.height = window->getHeight();
    state.touchType = OIS::MT_Moved;
    
    int last = state.X.abs;
    state.X.abs =  (int)x;
    state.X.rel = state.X.abs - last;
    
    last = state.Y.abs;
    state.Y.abs = (int)y;
    state.Y.rel = state.Y.abs - last;
    
    state.Z.abs = 0;
    state.Z.rel = 0;
    
    OIS::MultiTouchEvent event(this, state);
    
    std::vector<std::shared_ptr<Component>> components = componentMgr->getComponentsByType(&typeid(InputComponent));
    
    for (std::shared_ptr<Component> component : components)
    {
        dynamic_cast<InputComponent*>(component.get())->touchMoved(event);
    }
}

void iOSInputSystem::touchEnded(float x, float y)
{
    OIS::MultiTouchState state;
    state.width = window->getWidth();
    state.height = window->getHeight();
    state.touchType = OIS::MT_Released;
    
    int last = state.X.abs;
    state.X.abs =  (int)x;
    state.X.rel = state.X.abs - last;
    
    last = state.Y.abs;
    state.Y.abs = (int)y;
    state.Y.rel = state.Y.abs - last;
    
    state.Z.abs = 0;
    state.Z.rel = 0;
    
    OIS::MultiTouchEvent event(this, state);
    
    std::vector<std::shared_ptr<Component>> components = componentMgr->getComponentsByType(&typeid(InputComponent));
    
    for (std::shared_ptr<Component> component : components)
    {
        dynamic_cast<InputComponent*>(component.get())->touchReleased(event);
    }
}
