#include "Engine.h"

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
#define WIN32_LEAN_AND_MEAN
#include "windows.h"
#elif OGRE_PLATFORM == OGRE_PLATFORM_ANDROID
#include <android_native_app_glue.h>
#elif OGRE_PLATFORM == OGRE_PLATFORM_APPLE_IOS
#include "iOSBridge.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
    INT WINAPI WinMain( HINSTANCE hInst, HINSTANCE, LPSTR strCmdLine, INT )
#elif OGRE_PLATFORM == OGRE_PLATFORM_ANDROID
	void android_main(struct android_app* state)
#else
    int main(int argc, char *argv[])
#endif
    {
	#if OGRE_PLATFORM == OGRE_PLATFORM_APPLE_IOS
		NSAutoreleasePool* pool = [[NSAutoreleasePool alloc] init];
		int retVal = UIApplicationMain(argc, argv, @"UIApplication", @"AppDelegate");
		[pool release];
		return retVal;
	#else
		// Create application object
		Engine app;

	#if OGRE_PLATFORM == OGRE_PLATFORM_ANDROID
		app_dummy();
		app.setAndroidAppState(state);
	#endif

		try
		{
			// Jump to the engine class
			app.run();
		}
		catch (Ogre::Exception& e)
		{
	#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
			MessageBoxA(NULL, e.getFullDescription().c_str(), "An exception has occurred!", MB_ICONERROR | MB_TASKMODAL);
	#else
			std::cerr << "An exception has occurred: " << e.getFullDescription().c_str() << std::endl;
	#endif
		}
		
	#if OGRE_PLATFORM != OGRE_PLATFORM_ANDROID
		return 0;
	#endif
	
		#endif
	}

#ifdef __cplusplus
}
#endif
