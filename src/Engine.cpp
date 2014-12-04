#include "Engine.h"

#if OGRE_PLATFORM == OGRE_PLATFORM_ANDROID
	/* Sadly we need this because the android handles are static functions */
	static Engine* engine = NULL;
#endif

Engine::Engine()
	: root(0),
	  window(0),
	  logMgr(0),
	  appManager(0),
	  eventManager(0),
	  eventLogger(0),
	  timer(0),
	  config(0)
#if OGRE_PLATFORM == OGRE_PLATFORM_APPLE_IOS
#else
      ,
	  gui(0),
	  platform(0)
#endif
	  #if OGRE_PLATFORM == OGRE_PLATFORM_ANDROID || OGRE_PLATFORM == OGRE_PLATFORM_APPLE_IOS
	  ,
	  staticPluginLoader(0)
	  #endif
{
#if OGRE_PLATFORM == OGRE_PLATFORM_ANDROID
	if (engine == NULL)
		engine = this;
#endif
}

Engine::~Engine()
{
	appManager.reset();
	eventLogger.reset();
#if OGRE_PLATFORM == OGRE_PLATFORM_APPLE_IOS
#else
	gui->shutdown();
	platform->shutdown();
#endif
	#if OGRE_PLATFORM == OGRE_PLATFORM_ANDROID || OGRE_PLATFORM == OGRE_PLATFORM_APPLE_IOS
	SAFE_DELETE (staticPluginLoader);
	#endif
    #if OGRE_PLATFORM == OGRE_PLATFORM_ANDROID
	engine = NULL;
	#endif
	// !!! Don't log something after root is destroyed !!!
	SAFE_DELETE (overlaySystem);
	SAFE_DELETE (root);
	SAFE_DELETE (logMgr);
}

#if OGRE_PLATFORM == OGRE_PLATFORM_ANDROID
static int32_t handleInput(struct android_app* app, AInputEvent* event) 
{
            if (engine->getAppManager()->getActiveScene()->getTouchInput())
            {
                if (AInputEvent_getType(event) == AINPUT_EVENT_TYPE_MOTION)
                {
                    int action = (int)(AMOTION_EVENT_ACTION_MASK & AMotionEvent_getAction(event));

                    if(action == 0)
                    	engine->getAppManager()->getActiveScene()->getTouchInput()->injectTouchEvent(2, AMotionEvent_getRawX(event, 0), AMotionEvent_getRawY(event, 0) );

                    engine->getAppManager()->getActiveScene()->getTouchInput()->injectTouchEvent(action, AMotionEvent_getRawX(event, 0), AMotionEvent_getRawY(event, 0) );
                }
                else
                {
                	engine->getAppManager()->getActiveScene()->getTouchInput()->injectKeyEvent(AKeyEvent_getAction(event), AKeyEvent_getKeyCode(event));
                }

                return 1;
            }
            return 0;
}

static void handleCmd(struct android_app* app, int32_t cmd)
{
    switch (cmd) 
    {
        case APP_CMD_SAVE_STATE:
            break;
        case APP_CMD_INIT_WINDOW:
            if(app->window)
            {
				AConfiguration* config = AConfiguration_new();
				AConfiguration_fromAssetManager(config, app->activity->assetManager);
				engine->setAndroidConfig(config);
                if(!engine->getWindow())
                {
					engine->initWindow();
					engine->setupAvailableScenes();

                }
                else
                {
					static_cast<Ogre::AndroidEGLWindow*>(engine->getWindow())->_createInternalResources(app->window, config);
                }
                AConfiguration_delete(config);
            }
            break;
        case APP_CMD_TERM_WINDOW:
            if(engine->getWindow())
				static_cast<Ogre::AndroidEGLWindow*>(engine->getWindow())->_destroyInternalResources();
            break;
        case APP_CMD_GAINED_FOCUS:
            break;
        case APP_CMD_LOST_FOCUS:
            break;
    }
}
#endif

void Engine::run()
{
	// The ogre root element is needed by almost every subsystem
	root = new Ogre::Root("", "", "");
	overlaySystem = new Ogre::OverlaySystem();

	// Setup the logging systems for desktop platforms
#if OGRE_PLATFORM != OGRE_PLATFORM_APPLE_IOS && OGRE_PLATFORM != OGRE_PLATFORM_ANDROID
	logMgr = new Ogre::LogManager();
	logMgr->createLog("engine.log", true, true, false);
#endif

	// Setup event manager early as it is the most important subsystem
	eventManager = std::shared_ptr<EventManager>(new EventManager("MainEventManager", true));

	// Event logger listens to all events and logs them
	eventLogger = EventListenerPtr(new EventLogger());
	safeAddListener(eventLogger.get(), EventType(wildcardEventType));

	// Read in the config file that is needed for loading plugins and resources and initializing the rendering window
	config = std::shared_ptr<ConfigLoader>(new ConfigLoader("options.cfg"));
#if OGRE_PLATFORM == OGRE_PLATFORM_ANDROID
	if(!config->readConfig(state))
#else
	if(!config->readConfig())
#endif
		return;

	// Load plugins and resources defined by the config file
	if(!loadPluginsAndResources())
		return;

	// Init app manager that handles the different scenes
	appManager = std::shared_ptr<ApplicationManager>(new ApplicationManager(config));
	appManager->setOverlaySystem(overlaySystem);

	// Android will call these functions by the app handles, iOS has its own functionality in iOSBridge.h
#if OGRE_PLATFORM != OGRE_PLATFORM_APPLE_IOS && OGRE_PLATFORM != OGRE_PLATFORM_ANDROID
	if(!initWindow())
		return;
	if(!setupAvailableScenes())
		return;
#endif

	// Setup timer
	timer = std::shared_ptr<Timer>(new Timer(root->getTimer()->getMilliseconds()));

	LOG_DEBUG("Engine started");

#if OGRE_PLATFORM == OGRE_PLATFORM_ANDROID
	state->onAppCmd = &handleCmd;
	state->onInputEvent = &handleInput;
#endif

	// Start main loop, iOS has its own loop functionality in iOSBridge.h
#if OGRE_PLATFORM != OGRE_PLATFORM_APPLE_IOS
	// And go!
	startMainLoop();
#endif
}

bool Engine::loadPluginsAndResources()
{
#ifdef OGRE_STATIC_LIB
	staticPluginLoader = new Ogre::StaticPluginLoader();
	staticPluginLoader->load();
#else
	for (auto i : config->getPlugins())
		root->loadPlugin("/usr/local/lib/OGRE/" + i);
#endif

	LOG_DEBUG("Plugins loaded");

#if OGRE_PLATFORM == OGRE_PLATFORM_ANDROID
	Ogre::ArchiveManager::getSingleton().addArchiveFactory( new Ogre::APKFileSystemArchiveFactory(state->activity->assetManager) );
	Ogre::ArchiveManager::getSingleton().addArchiveFactory( new Ogre::APKZipArchiveFactory(state->activity->assetManager) );
#endif

	std::string name;
	std::string type;
	std::string group;

	if (config->getOptions()["resourceLoading"] == "fileSystem")
#if OGRE_PLATFORM == OGRE_PLATFORM_ANDROID
		type = "APKFileSystem";
#else
		type = "FileSystem";
#endif
	else
#if OGRE_PLATFORM == OGRE_PLATFORM_ANDROID
		type = "APKZip";
#else
		type = "Zip";
#endif

	Ogre::NameValuePairList::const_iterator resourceIt;
	Ogre::NameValuePairList resources = config->getResources();

	// Load resources
	for (resourceIt=resources.begin(); resourceIt!=resources.end(); resourceIt++)
	{
	#if OGRE_PLATFORM == OGRE_PLATFORM_LINUX
		name = "assets" + resourceIt->first;
	#elif OGRE_PLATFORM == OGRE_PLATFORM_APPLE_IOS
		name = Ogre::macBundlePath() + "/" + resourceIt->first;
	#else
		name = resourceIt->first;
	#endif
		group = resourceIt->second;
		Ogre::ResourceGroupManager::getSingleton().addResourceLocation(name, type, group);
	}

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
	Ogre::ResourceGroupManager::getSingleton().addResourceLocation(
		"c:\\windows\\fonts", "FileSystem", "GUI");
#endif

	//Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();
	Ogre::ResourceGroupManager::getSingleton().initialiseResourceGroup("General");
	Ogre::ResourceGroupManager::getSingleton().initialiseResourceGroup("GUI");
	Ogre::ResourceGroupManager::getSingleton().initialiseResourceGroup("Scenes");
	Ogre::ResourceGroupManager::getSingleton().initialiseResourceGroup("Scripts");

	LOG_DEBUG("Resources loaded");

	return true;
}

bool Engine::initWindow()
{
	const Ogre::RenderSystemList &renderSystems = root->getAvailableRenderers();

	// Check if a renderer is available
	if(renderSystems.empty())
	{
		LOG_ERROR("Empty renderer list. Exiting...");
		return false;
	}

	unsigned int height, width;
	bool fullscreen = false;
	Ogre::NameValuePairList options;

	// Set resolution and render system platform specific
#if OGRE_PLATFORM == OGRE_PLATFORM_ANDROID

	height = 0;
	width = 0;
	fullscreen = false;

	root->setRenderSystem(renderSystems.at(0));

	options["externalWindowHandle"] = Ogre::StringConverter::toString((int)state->window);
	options["androidConfig"] = Ogre::StringConverter::toString((int)androidConfig);

#elif OGRE_PLATFORM == OGRE_PLATFORM_APPLE_IOS

	height = 768;
	width = 1024;
	fullscreen = true;

    root->setRenderSystem(renderSystems.at(0));

#else

	Ogre::RenderSystemList::const_iterator r_it;

	std::string value = config->getOptions().find("renderSystem")->second;

	// Go through all available renderers and search for the one from the config file
	bool renderSystemFound = false;
	for (r_it=renderSystems.begin(); r_it!=renderSystems.end(); r_it++)
	{
		Ogre::RenderSystem *tmp = *r_it;
		std::string rName(tmp->getName());

		// returns -1 if string not found
		if ((int) rName.find(value) >= 0)
		{
			root->setRenderSystem(*r_it);
			renderSystemFound = true;
			break;
		}
	}

	// Exiting when no renderer is found
	if (!renderSystemFound)
	{
		LOG_ERROR("Specified render system (" + value + ") not found, exiting...");
		return false;
	}

	value = config->getOptions().find("resolution")->second;
	sscanf(value.c_str(), "%dx%d", &width, &height);

	value = config->getOptions().find("fullscreen")->second;
	if (value == "true")
		fullscreen = true;

	options = config->getOptions();

#endif

	// Create window manually
	root->initialise(false);
	window = root->createRenderWindow(config->getOptions().find("appName")->second, width, height, fullscreen, &options);
	
	if (!root->isInitialised())
		LOG_ERROR("RenderSystem not initialised.");

	LOG_DEBUG("Init Window");

	this->setupGuiSystem();

    return true;
}

bool Engine::setupGuiSystem()
{
#if OGRE_PLATFORM == OGRE_PLATFORM_APPLE_IOS
#else
	// setup GUI system
	platform = std::shared_ptr<MyGUI::OgrePlatform>(new MyGUI::OgrePlatform());
	platform->initialise(0, 0, "GUI", "");
	gui = std::shared_ptr<MyGUI::Gui>(new MyGUI::Gui());
	gui->initialise("MainGuiComponents.xml");
#if OGRE_PLATFORM == OGRE_PLATFORM_ANDROID
	MyGUI::PointerManager::getInstance().setVisible(false);
#endif
	LOG_DEBUG("GUI System loaded")
#endif
	return true;
}

bool Engine::setupAvailableScenes()
{
	appManager->setRenderWindow(window);

	if (config->getOptions()["startScene"].empty())
	{
		LOG_ERROR("No valid start scene found in config. Exiting...");
		return false;
	}

	appManager->changeScene(config->getOptions().find("startScene")->second);

	LOG_DEBUG("Scenes loaded")

	return true;
}

bool Engine::startMainLoop()
{
	LOG_DEBUG("MainLoop started");

#if OGRE_PLATFORM == OGRE_PLATFORM_ANDROID
	int ident, events;
	struct android_poll_source* source;
#endif

	// Process all events before starting loop
	// Important for init gameobjects and components
	safeTickEventManager();

	while (!appManager->isShutdown())
	{
	#if OGRE_PLATFORM == OGRE_PLATFORM_ANDROID
		while ((ident = ALooper_pollAll(0, NULL, &events, (void**)&source)) >= 0)
		{
			if (source != NULL)
				source->process(state, source);

			if (state->destroyRequested != 0)
				return true;
		}
	#else
		if (window->isClosed())
			appManager->shutdownEngine();
	#endif

		Ogre::WindowEventUtilities::messagePump();

		if (window != NULL && window->isActive())
		{

			window->windowMovedOrResized();
			timer->Update(root->getTimer()->getMilliseconds());

			if (appManager != NULL && appManager->getActiveScene() != 0)
				appManager->getActiveScene()->update(timer->getElapsed());

			safeTickEventManager(100);
			if (appManager->changedScene)
			{
				safeTickEventManager();
				appManager->changedScene = false;
				LOG_DEBUG("All entities initialised")
			}
			else
			{
				root->renderOneFrame();
			}
		}
		else
		{
			#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
				Sleep(1000);
			#elif OGRE_PLATFORM == OGRE_PLATFORM_ANDROID
			#else
				sleep(1);
			#endif
		}
	}

	LOG_DEBUG("MainLoop stopped")

	return true;
}
