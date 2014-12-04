#pragma once

#include "OgreRoot.h"
#include "OgrePlatform.h"
#include "Engine.h"

#import <UIKit/UIKit.h> 
#import <QuartzCore/QuartzCore.h>
#import "InputGestureView.h"

#ifdef __OBJC__

@interface AppDelegate : NSObject <UIApplicationDelegate>
{
    Engine engine;

    CADisplayLink *displayLink;
    NSDate* date;
    NSTimeInterval lastFrameTime;
}

- (void)go;
- (void)renderOneFrame:(id)sender;

@property (nonatomic) NSTimeInterval lastFrameTime;

@end

@implementation AppDelegate

@dynamic lastFrameTime;

- (NSTimeInterval)lastFrameTime
{
    return lastFrameTime;
}

- (void)setLastFrameTime:(NSTimeInterval)frameInterval
{
    if (frameInterval >= 1)
    {
        lastFrameTime = frameInterval;
    }
}

- (void)go {

    NSAutoreleasePool * pool = [[NSAutoreleasePool alloc] init];

    try {
        engine.run();

        Ogre::Root::getSingleton().getRenderSystem()->_initRenderTargets();
        
        engine.initWindow();
        engine.setupAvailableScenes();
        safeTickEventManager(1000);

		Ogre::Root::getSingleton().clearEventTimes();
    } catch( Ogre::Exception& e ) {
        std::cerr << "An exception has occurred: " <<
        e.getFullDescription().c_str() << std::endl;
    }

    [pool release];
}

- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions
{
    // Defaulting to 2 means that we run at 30 frames per second. For 60 frames, use a value of 1.
    // 30 FPS is usually sufficient and results in lower power consumption.
    lastFrameTime = 2;
    displayLink = nil;

    [self go];

    return YES;
}

- (void)applicationWillTerminate:(UIApplication *)application
{

}

- (void)applicationDidBecomeActive:(UIApplication *)application
{
    Ogre::Root::getSingleton().clearEventTimes();
    date = [[NSDate alloc] init];
    lastFrameTime = 2; // Reset the timer

    displayLink = [CADisplayLink displayLinkWithTarget:self selector:@selector(renderOneFrame:)];
    [displayLink setFrameInterval:lastFrameTime];
    [displayLink addToRunLoop:[NSRunLoop currentRunLoop] forMode:NSDefaultRunLoopMode];
}

- (void)applicationWillResignActive:(UIApplication *)application
{
    Ogre::Root::getSingleton().saveConfig();

    [date release];
    date = nil;
    
    [displayLink invalidate];
    displayLink = nil;
}

- (void)renderOneFrame:(id)sender
{
    // NSTimeInterval is a simple typedef for double
    NSTimeInterval currentFrameTime = -[date timeIntervalSinceNow];
    NSTimeInterval differenceInSeconds = currentFrameTime - lastFrameTime;
    lastFrameTime = currentFrameTime;

    dispatch_async(dispatch_get_main_queue(), ^(void)
	{
		if (engine.getAppManager() != NULL && engine.getAppManager()->getActiveScene() != 0)
			engine.getAppManager()->getActiveScene()->update((Ogre::Real)differenceInSeconds);

		safeTickEventManager(100);

		if (engine.getAppManager()->changedScene)
		{
			safeTickEventManager(1000);
			engine.getAppManager()->changedScene = false;
		}

		Ogre::Root::getSingleton().renderOneFrame((Ogre::Real)differenceInSeconds);
	});
}

@end

#endif

