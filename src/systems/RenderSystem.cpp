#include "RenderSystem.h"

void RenderSystem::start()
{
	safeAddListener(dynamic_cast<IEventListener*>(this), EventType("ComponentAdded"));
	safeAddListener(dynamic_cast<IEventListener*>(this), EventType("EntityDestroyed"));
	safeAddListener(dynamic_cast<IEventListener*>(this), EventType("RemoveComponent"));
	safeAddListener(dynamic_cast<IEventListener*>(this), EventType("FadeIn"));
	safeAddListener(dynamic_cast<IEventListener*>(this), EventType("FadeOut"));
	safeAddListener(dynamic_cast<IEventListener*>(this), EventType("ShowOverlay"));
	safeAddListener(dynamic_cast<IEventListener*>(this), EventType("HideOverlay"));

	std::string overlay = "Overlays/FadeInOut";
	std::string material = "Materials/OverlayMaterial";
#if OGRE_PLATFORM == OGRE_PLATFORM_ANDROID || OGRE_PLATFORM == OGRE_PLATFORM_APPLE_IOS
	overlay = "Overlays/FadeInOutAndroid";
	material = "Materials/OverlayMaterialAndroid";
#endif
	fader = std::shared_ptr<Fader>(new Fader(overlay.c_str(), material.c_str()));
	LOG_DEBUG("Render system started");
}

void RenderSystem::update(float deltaTime)
{
	std::vector<std::shared_ptr<Component>> components = componentMgr->getComponentsByType(&typeid(RenderComponent));

	for (std::shared_ptr<Component> component : components)
	{
		Ogre::Vector3 position = dynamic_cast<TransformComponent*>(componentMgr->getComponent(component->entityID, &typeid(TransformComponent)).get())->getPosition();
		Ogre::Quaternion rotation = dynamic_cast<TransformComponent*>(componentMgr->getComponent(component->entityID, &typeid(TransformComponent)).get())->getRotation();
		Ogre::Vector3 scale = dynamic_cast<TransformComponent*>(componentMgr->getComponent(component->entityID, &typeid(TransformComponent)).get())->getScale();

		RenderComponent* renderComponent = dynamic_cast<RenderComponent*>(component.get());
		if (renderComponent->node) {
			renderComponent->node->setPosition(position);
			renderComponent->node->setOrientation(rotation);
			renderComponent->node->setScale(scale);
		}
	}

	fader->fade(deltaTime);
}

void RenderSystem::stop()
{
	safeDelListener(dynamic_cast<IEventListener*>(this), EventType("ComponentAdded"));
	safeDelListener(dynamic_cast<IEventListener*>(this), EventType("EntityDestroyed"));
	safeDelListener(dynamic_cast<IEventListener*>(this), EventType("RemoveComponent"));
	safeDelListener(dynamic_cast<IEventListener*>(this), EventType("FadeIn"));
	safeDelListener(dynamic_cast<IEventListener*>(this), EventType("FadeOut"));
	safeDelListener(dynamic_cast<IEventListener*>(this), EventType("ShowOverlay"));
	safeDelListener(dynamic_cast<IEventListener*>(this), EventType("HideOverlay"));
	LOG_DEBUG("Render system stopped");
}

bool RenderSystem::handleEvent(const GameEvent& event)
{
	if (event.getEventType() == EventType("ComponentAdded"))
	{
		const ComponentEvent& componentEvent = dynamic_cast<const ComponentEvent&>(event);
		if (componentEvent.componentType == &typeid(RenderComponent))
			initRenderComponent(componentMgr->getComponent(componentEvent.entityID, &typeid(RenderComponent)));
	}
	else if (event.getEventType() == EventType("EntityDestroyed"))
	{
		const EntityEvent& entityEvent = dynamic_cast<const EntityEvent&>(event);
		if (componentMgr->getComponent(entityEvent.entityID, &typeid(RenderComponent)))
			removeRenderComponent(componentMgr->getComponent(entityEvent.entityID, &typeid(RenderComponent)));
	}
	else if (event.getEventType() == EventType("RemoveComponent"))
	{
		const ComponentEvent& componentEvent = dynamic_cast<const ComponentEvent&>(event);
		if (componentEvent.componentType == &typeid(RenderComponent))
			removeRenderComponent(componentMgr->getComponent(componentEvent.entityID, &typeid(RenderComponent)));
	}
	else if (event.getEventType() == EventType("FadeIn"))
	{
		const FloatEvent& castedEvent = dynamic_cast<const FloatEvent&>(event);
		fader->startFadeIn(castedEvent.value);
	}
	else if (event.getEventType() == EventType("FadeOut"))
	{
		const FloatEvent& castedEvent = dynamic_cast<const FloatEvent&>(event);
		fader->startFadeOut(castedEvent.value);
	}
	else if (event.getEventType() == EventType("ShowOverlay"))
	{
		const SceneEvent& castedEvent = dynamic_cast<const SceneEvent&>(event);
		Ogre::OverlayManager::getSingleton().getByName(castedEvent.sceneName)->show();
	}
	else if (event.getEventType() == EventType("HideOverlay"))
	{
		const SceneEvent& castedEvent = dynamic_cast<const SceneEvent&>(event);
		Ogre::OverlayManager::getSingleton().getByName(castedEvent.sceneName)->hide();
	}

	return true;
}

void RenderSystem::initRenderComponent(std::shared_ptr<Component> component)
{
	RenderComponent* renderComponent = dynamic_cast<RenderComponent*>(component.get());

	Ogre::MovableObject* object = 0;
	if (componentMgr->getComponent(renderComponent->entityID, &typeid(MeshComponent)))
		object = dynamic_cast<MeshComponent*>(componentMgr->getComponent(renderComponent->entityID, &typeid(MeshComponent)).get())->entity;
	else if (componentMgr->getComponent(renderComponent->entityID, &typeid(CameraComponent)))
		object = dynamic_cast<CameraComponent*>(componentMgr->getComponent(renderComponent->entityID, &typeid(CameraComponent)).get())->camera;
	else if (componentMgr->getComponent(renderComponent->entityID, &typeid(LightComponent)))
		object = dynamic_cast<LightComponent*>(componentMgr->getComponent(renderComponent->entityID, &typeid(LightComponent)).get())->light;

	renderComponent->node = sceneMgr->getRootSceneNode()->createChildSceneNode(renderComponent->nodeName);
	if(object != 0)
		renderComponent->node->attachObject(object);

	Ogre::Vector3 position = dynamic_cast<TransformComponent*>(componentMgr->getComponent(renderComponent->entityID, &typeid(TransformComponent)).get())->getPosition();
	Ogre::Quaternion rotation = dynamic_cast<TransformComponent*>(componentMgr->getComponent(renderComponent->entityID, &typeid(TransformComponent)).get())->getRotation();
	Ogre::Vector3 scale = dynamic_cast<TransformComponent*>(componentMgr->getComponent(renderComponent->entityID, &typeid(TransformComponent)).get())->getScale();

	renderComponent->node->setPosition(position);
	renderComponent->node->setOrientation(rotation);
	renderComponent->node->setScale(scale);
}

void RenderSystem::removeRenderComponent(std::shared_ptr<Component> component)
{
	RenderComponent* renderComponent = dynamic_cast<RenderComponent*>(component.get());

	sceneMgr->destroySceneNode(renderComponent->node);
	renderComponent->node = 0;

	componentMgr->removeComponent(renderComponent->entityID, &typeid(RenderComponent));
}
