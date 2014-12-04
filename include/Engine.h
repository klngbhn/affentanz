#pragma once

#include <memory>

#include "Ogre.h"
#include "OgreOverlaySystem.h"
#if OGRE_PLATFORM == OGRE_PLATFORM_APPLE_IOS
#else
#include "MyGUI.h"
#include "MyGUI_OgrePlatform.h"
#endif

#if OGRE_PLATFORM == OGRE_PLATFORM_LINUX
	#include <OgreWindowEventUtilities.h>
#endif

#include "ConfigLoader.h"
#include "ApplicationManager.h"
#include "EventManager.h"
#include "EventLogger.h"
#include "Timer.h"
#include "Functions.h"
#include "Log.h"

#if OGRE_PLATFORM == OGRE_PLATFORM_ANDROID
	#define OGRE_STATIC_GLES2
	#define OGRE_STATIC_ParticleFX
	#define OGRE_STATIC_OctreeSceneManager

	#include <android_native_app_glue.h>
	#include <EGL/egl.h>
	#include "OgreStaticPluginLoader.h"
	#include "Android/OgreAndroidEGLWindow.h"
	#include "Android/OgreAPKFileSystemArchive.h"
	#include "Android/OgreAPKZipArchive.h"
	#include "TouchInputSystem.h"
#endif

#if OGRE_PLATFORM == OGRE_PLATFORM_APPLE_IOS
	#define OGRE_STATIC_GLES2
	#define OGRE_STATIC_ParticleFX
	#define OGRE_STATIC_OctreeSceneManager

	#include <iOS/macUtils.h>
	#include "OgreStaticPluginLoader.h"
#endif

#ifdef WIN32
	#include <shlobj.h>
#endif

/*
 * The engine class is the heart of the framework. It starts all systems (rendering, gui, logging, events, ...) and
 * reads the initial config options from file. Then it runs the main loop and shuts down everything in the end.
 */
class Engine
{
public:
	Engine();
	virtual ~Engine();
	void run();
#if OGRE_PLATFORM == OGRE_PLATFORM_ANDROID
	void setAndroidAppState(struct android_app* state) { this->state = state; }
	void setAndroidConfig(AConfiguration* config) { this->androidConfig = config; }
#endif

	bool initWindow();
	bool setupAvailableScenes();
	Ogre::RenderWindow* getWindow() { return window; }
	std::shared_ptr<ApplicationManager> getAppManager() { return appManager; }
private:
	bool loadPluginsAndResources();
	bool startMainLoop();
	bool setupGuiSystem();

	Ogre::Root* root;
	Ogre::OverlaySystem* overlaySystem;
	Ogre::LogManager* logMgr;
	Ogre::RenderWindow* window;

#if OGRE_PLATFORM == OGRE_PLATFORM_APPLE_IOS
#else
	std::shared_ptr<MyGUI::Gui> gui;
	std::shared_ptr<MyGUI::OgrePlatform> platform;
#endif

	std::shared_ptr<ApplicationManager> appManager;
	std::shared_ptr<ConfigLoader> config;
	std::shared_ptr<EventManager> eventManager;
	std::shared_ptr<Timer> timer;
	EventListenerPtr eventLogger;
	
#ifdef OGRE_STATIC_LIB
    Ogre::StaticPluginLoader* staticPluginLoader;
#endif

#if OGRE_PLATFORM == OGRE_PLATFORM_ANDROID
	struct android_app* state;
	AConfiguration* androidConfig;
#endif
};

