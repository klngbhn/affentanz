#include "LightSystem.h"

void LightSystem::start()
{
	safeAddListener(dynamic_cast<IEventListener*>(this), EventType("ComponentAdded"));
	safeAddListener(dynamic_cast<IEventListener*>(this), EventType("EntityDestroyed"));
	safeAddListener(dynamic_cast<IEventListener*>(this), EventType("RemoveComponent"));
	LOG_DEBUG("Light system started");
}

void LightSystem::update(float deltaTime)
{

}

void LightSystem::stop()
{
	safeDelListener(dynamic_cast<IEventListener*>(this), EventType("ComponentAdded"));
	safeDelListener(dynamic_cast<IEventListener*>(this), EventType("EntityDestroyed"));
	safeDelListener(dynamic_cast<IEventListener*>(this), EventType("RemoveComponent"));
	LOG_DEBUG("Light system stopped");
}

bool LightSystem::handleEvent(const GameEvent& event)
{
	if (event.getEventType() == EventType("ComponentAdded"))
	{
		const ComponentEvent& componentEvent = dynamic_cast<const ComponentEvent&>(event);
		if (componentEvent.componentType == &typeid(LightComponent))
			initLightComponent(componentMgr->getComponent(componentEvent.entityID, &typeid(LightComponent)));
	}
	else if (event.getEventType() == EventType("EntityDestroyed"))
	{
		const EntityEvent& entityEvent = dynamic_cast<const EntityEvent&>(event);
		if (componentMgr->getComponent(entityEvent.entityID, &typeid(LightComponent)))
			removeLightComponent(componentMgr->getComponent(entityEvent.entityID, &typeid(LightComponent)));
	}
	else if (event.getEventType() == EventType("RemoveComponent"))
	{
		const ComponentEvent& componentEvent = dynamic_cast<const ComponentEvent&>(event);
		if (componentEvent.componentType == &typeid(LightComponent))
			removeLightComponent(componentMgr->getComponent(componentEvent.entityID, &typeid(LightComponent)));
	}

	return true;
}

void LightSystem::initLightComponent(std::shared_ptr<Component> component)
{
	LightComponent* lightComponent = dynamic_cast<LightComponent*>(component.get());

	//Ogre::Vector3 position = dynamic_cast<TransformComponent*>(componentMgr->getComponent(lightComponent->entityID, &typeid(TransformComponent)).get())->getPosition();

	lightComponent->light = sceneMgr->createLight(lightComponent->lightName);
	lightComponent->light->setType(lightComponent->lightType);
	//lightComponent->light->setPosition(position);
	lightComponent->light->setDiffuseColour(lightComponent->diffuseColor);
	lightComponent->light->setSpecularColour(lightComponent->specularColor);
	lightComponent->light->setDirection(lightComponent->direction);
}

void LightSystem::removeLightComponent(std::shared_ptr<Component> component)
{
	LightComponent* lightComponent = dynamic_cast<LightComponent*>(component.get());

	sceneMgr->destroyLight(lightComponent->light);
	lightComponent->light = 0;

	componentMgr->removeComponent(lightComponent->entityID, &typeid(LightComponent));
}
