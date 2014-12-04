#include <ApplicationManager.h>
#include "Log.h"

ApplicationManager::ApplicationManager(std::shared_ptr<ConfigLoader> config)
{
	this->config = config;
	shutdown = false;
	changedScene = false;
	safeAddListener(dynamic_cast<IEventListener*>(this), EventType("QuitApplication"));
	safeAddListener(dynamic_cast<IEventListener*>(this), EventType("ChangeScene"));
}

ApplicationManager::~ApplicationManager()
{
	safeDelListener(dynamic_cast<IEventListener*>(this), EventType("ChangeScene"));
	safeDelListener(dynamic_cast<IEventListener*>(this), EventType("QuitApplication"));

	if(!activeSceneStack.empty())
	{
		activeSceneStack.back()->exit();
		activeSceneStack.pop_back();
	}
}

bool ApplicationManager::handleEvent(const GameEvent& event)
{
	if (event.getEventType() == EventType("QuitApplication"))
	{
		while(!activeSceneStack.empty())
		{
			activeSceneStack.back()->exit();
			activeSceneStack.pop_back();
		}
		shutdown = true;
	}
	else if (event.getEventType() == EventType("ChangeScene"))
	{
		const SceneEvent& sceneEvent = dynamic_cast<const SceneEvent&>(event);
		this->changeScene(sceneEvent.sceneName);
	}

	return true;
}

void ApplicationManager::addSceneToManager(Ogre::String sceneName)
{
	availableScenes.push_back(sceneName);
}

void ApplicationManager::changeScene(Ogre::String sceneName)
{
	//if sceneName is in availableScenes
	if(!activeSceneStack.empty())
	{
		activeSceneStack.back()->exit();
		activeSceneStack.pop_back();
	}
	ScenePtr scene(new DotScene(sceneName));
	scene->setRenderWindow(window);
	scene->setConfig(config);
	scene->setOverlaySystem(overlaySystem);
	activeSceneStack.push_back(scene);
	activeSceneStack.back()->enter();
	changedScene = true;
}

bool ApplicationManager::pushScene(Ogre::String sceneName)
{
	if(!activeSceneStack.empty())
	{
		if(!activeSceneStack.back()->pause())
			return false;
	}

	ScenePtr scene(new DotScene(sceneName));
	scene->setRenderWindow(window);
	scene->setConfig(config);

	activeSceneStack.push_back(scene);
	activeSceneStack.back()->enter();
	changedScene = true;

	return true;
}

void ApplicationManager::popScene()
{
	if(!activeSceneStack.empty())
	{
		activeSceneStack.back()->exit();
		activeSceneStack.pop_back();
	}

	if(!activeSceneStack.empty())
	{
		activeSceneStack.back()->resume();
	}
    else
		shutdown = true;
}

void ApplicationManager::popAllAndPushScene(Ogre::String sceneName)
{
    while(!activeSceneStack.empty())
    {
    	activeSceneStack.back()->exit();
    	activeSceneStack.pop_back();
    }

    pushScene(sceneName);
}

ScenePtr ApplicationManager::getActiveScene()
{
	if(!activeSceneStack.empty())
	{
		return activeSceneStack.back();
	}

	return 0;
}

void ApplicationManager::pauseScene()
{
	if(!activeSceneStack.empty())
	{
		activeSceneStack.back()->pause();
	}

	if(activeSceneStack.size() > 2)
	{
		activeSceneStack.at(activeSceneStack.size() - 2)->resume();
	}
}

