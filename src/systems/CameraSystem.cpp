#include "CameraSystem.h"

void CameraSystem::start()
{
	safeAddListener(dynamic_cast<IEventListener*>(this), EventType("ComponentAdded"));
	safeAddListener(dynamic_cast<IEventListener*>(this), EventType("EntityDestroyed"));
	safeAddListener(dynamic_cast<IEventListener*>(this), EventType("RemoveComponent"));
	safeAddListener(dynamic_cast<IEventListener*>(this), EventType("ChangePolygonMode"));
	safeAddListener(dynamic_cast<IEventListener*>(this), EventType("TakeScreenshot"));
	LOG_DEBUG("Camera system started");
}

void CameraSystem::update(float deltaTime)
{

}

void CameraSystem::stop()
{
	safeDelListener(dynamic_cast<IEventListener*>(this), EventType("ComponentAdded"));
	safeDelListener(dynamic_cast<IEventListener*>(this), EventType("EntityDestroyed"));
	safeDelListener(dynamic_cast<IEventListener*>(this), EventType("RemoveComponent"));
	safeDelListener(dynamic_cast<IEventListener*>(this), EventType("ChangePolygonMode"));
	safeDelListener(dynamic_cast<IEventListener*>(this), EventType("TakeScreenshot"));

#if OGRE_PLATFORM == OGRE_PLATFORM_APPLE_IOS
#else
	MyGUI::OgreRenderManager::getInstance().setSceneManager(0);
	MyGUI::OgreRenderManager::getInstance().setRenderWindow(0);
#endif

	sceneMgr->destroyAllCameras();
	window->removeAllViewports();

	LOG_DEBUG("Camera system stopped");
}

bool CameraSystem::handleEvent(const GameEvent& event)
{
	if (event.getEventType() == EventType("ComponentAdded"))
	{
		const ComponentEvent& componentEvent = dynamic_cast<const ComponentEvent&>(event);
		if (componentEvent.componentType == &typeid(CameraComponent))
			initCameraComponent(componentMgr->getComponent(componentEvent.entityID, &typeid(CameraComponent)));
	}
	else if (event.getEventType() == EventType("ChangePolygonMode"))
	{
		std::vector<std::shared_ptr<Component>> components = componentMgr->getComponentsByType(&typeid(CameraComponent));

		for (std::shared_ptr<Component> component : components)
		{
			Ogre::Camera* camera = dynamic_cast<CameraComponent*>(component.get())->camera;

			Ogre::PolygonMode pm;

			switch (camera->getPolygonMode())
			{
			case Ogre::PM_SOLID:
				pm = Ogre::PM_WIREFRAME;
				break;
			case Ogre::PM_WIREFRAME:
				pm = Ogre::PM_POINTS;
				break;
			default:
				pm = Ogre::PM_SOLID;
			}

			camera->setPolygonMode(pm);
		}
	}
	else if (event.getEventType() == EventType("TakeScreenshot"))
	{
		window->writeContentsToTimestampedFile("screenshot_", ".jpg");
	}
	else if (event.getEventType() == EventType("EntityDestroyed"))
	{
		const EntityEvent& entityEvent = dynamic_cast<const EntityEvent&>(event);
		if (componentMgr->getComponent(entityEvent.entityID, &typeid(CameraComponent)))
			removeCameraComponent(componentMgr->getComponent(entityEvent.entityID, &typeid(CameraComponent)));
	}
	else if (event.getEventType() == EventType("RemoveComponent"))
	{
		const ComponentEvent& componentEvent = dynamic_cast<const ComponentEvent&>(event);
		if (componentEvent.componentType == &typeid(CameraComponent))
			removeCameraComponent(componentMgr->getComponent(componentEvent.entityID, &typeid(CameraComponent)));
	}

	return true;
}

void CameraSystem::initCameraComponent(std::shared_ptr<Component> component)
{
	CameraComponent* cameraComponent = dynamic_cast<CameraComponent*>(component.get());

	cameraComponent->camera = sceneMgr->createCamera(cameraComponent->cameraName);
	cameraComponent->viewport = window->addViewport(cameraComponent->camera);
	cameraComponent->viewport->setBackgroundColour(cameraComponent->backgroundColor);
	cameraComponent->viewport->setOverlaysEnabled(true);

#if OGRE_PLATFORM == OGRE_PLATFORM_APPLE_IOS
#else
	MyGUI::OgreRenderManager::getInstance().setRenderWindow(window);
	MyGUI::OgreRenderManager::getInstance().setSceneManager(sceneMgr);
	MyGUI::OgreRenderManager::getInstance().setActiveViewport(window->getNumViewports() - 1);
#endif
    
	//Ogre::Vector3 position = dynamic_cast<TransformComponent*>(componentMgr->getComponent(cameraComponent->entityID, &typeid(TransformComponent)).get())->getPosition();
	//Ogre::Quaternion rotation = dynamic_cast<TransformComponent*>(componentMgr->getComponent(cameraComponent->entityID, &typeid(TransformComponent)).get())->getRotation();

	cameraComponent->camera->setNearClipDistance(cameraComponent->nearClipDistance);
	cameraComponent->camera->setFarClipDistance(cameraComponent->farClipDistance);
	//cameraComponent->camera->setPosition(position);
	//cameraComponent->camera->setOrientation(rotation);
//	cameraComponent->camera->lookAt(cameraComponent->lookAt);
	cameraComponent->camera->setAspectRatio(Ogre::Real(cameraComponent->viewport->getActualWidth()) / Ogre::Real(cameraComponent->viewport->getActualHeight()));

#if OGRE_PLATFORM == OGRE_PLATFORM_ANDROID
	cameraComponent->camera->setAutoAspectRatio(true);
#endif
}

void CameraSystem::removeCameraComponent(std::shared_ptr<Component> component)
{
	CameraComponent* cameraComponent = dynamic_cast<CameraComponent*>(component.get());

	sceneMgr->destroyCamera(cameraComponent->camera);
	window->removeViewport(window->getNumViewports() - 1);

	componentMgr->removeComponent(cameraComponent->entityID, &typeid(CameraComponent));
}
