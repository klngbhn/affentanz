#pragma once

#include "IEventManager.h"
#include "GameEvent.h"
#include "Scene.h"
#include "DotScene.h"
#include "Log.h"
#include "Functions.h"
#include "ConfigLoader.h"

class Scene;
typedef std::shared_ptr<Scene> ScenePtr;

/*
 * The ApplicationManager handles the change between two scenes.
 */
class ApplicationManager: public IEventListener
{
public:
	struct SceneInfo
	{
		Ogre::String name;
		ScenePtr scene;
	};

	ApplicationManager(std::shared_ptr<ConfigLoader> config);
    virtual ~ApplicationManager();

    void addSceneToManager(Ogre::String sceneName);

	void start(ScenePtr scene);
	void changeScene(Ogre::String sceneName);
	bool pushScene(Ogre::String sceneName);
	void popScene();
	void pauseScene();
	void popAllAndPushScene(Ogre::String sceneName);
	ScenePtr getActiveScene();

	std::string getName() { return "GameManagerListener"; }
	bool handleEvent(const GameEvent& event);

	void setRenderWindow(Ogre::RenderWindow* window) { this->window = window; }
	void setOverlaySystem(Ogre::OverlaySystem* overlaySystem) { this->overlaySystem = overlaySystem; }

	void shutdownEngine() { shutdown = true; }
    inline bool isShutdown() { return shutdown; }
    bool changedScene;
private:
    bool shutdown;

    Ogre::RenderWindow* window;
    Ogre::OverlaySystem* overlaySystem;
    std::shared_ptr<ConfigLoader> config;

    std::vector<ScenePtr>	activeSceneStack;
	std::vector<std::string>	availableScenes;
};
