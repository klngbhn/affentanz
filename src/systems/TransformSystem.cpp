#include "TransformSystem.h"

void TransformSystem::start()
{
	safeAddListener(dynamic_cast<IEventListener*>(this), EventType("EntityDestroyed"));
	safeAddListener(dynamic_cast<IEventListener*>(this), EventType("RemoveComponent"));
	safeAddListener(dynamic_cast<IEventListener*>(this), EventType("TranslateInDirection"));
	LOG_DEBUG("Transform system started");
}

void TransformSystem::update(float deltaTime)
{

}

void TransformSystem::stop()
{
	safeDelListener(dynamic_cast<IEventListener*>(this), EventType("EntityDestroyed"));
	safeDelListener(dynamic_cast<IEventListener*>(this), EventType("RemoveComponent"));
	safeDelListener(dynamic_cast<IEventListener*>(this), EventType("TranslateInDirection"));
	LOG_DEBUG("Transform system stopped");
}

bool TransformSystem::handleEvent(const GameEvent& event)
{
	if (event.getEventType() == EventType("EntityDestroyed"))
	{
		const EntityEvent& entityEvent = dynamic_cast<const EntityEvent&>(event);
		if (componentMgr->getComponent(entityEvent.entityID, &typeid(TransformComponent)))
			removeTransformComponent(componentMgr->getComponent(entityEvent.entityID, &typeid(TransformComponent)));
	}
	else if (event.getEventType() == EventType("RemoveComponent"))
	{
		const ComponentEvent& componentEvent = dynamic_cast<const ComponentEvent&>(event);
		if (componentEvent.componentType == &typeid(TransformComponent))
			removeTransformComponent(componentMgr->getComponent(componentEvent.entityID, &typeid(TransformComponent)));
	}
	else if (event.getEventType() == EventType("TranslateInDirection"))
	{
		const ForceEvent& forceEvent = dynamic_cast<const ForceEvent&>(event);
		TransformComponent* transform = dynamic_cast<TransformComponent*>(componentMgr->getComponent(forceEvent.entityID, &typeid(TransformComponent)).get());
		transform->setPosition(transform->getPosition() + (transform->getRotation() * Ogre::Vector3(forceEvent.directionX, forceEvent.directionY, forceEvent.directionZ)));
	}

	return true;
}

void TransformSystem::removeTransformComponent(std::shared_ptr<Component> component)
{
	TransformComponent* transformComponent = dynamic_cast<TransformComponent*>(component.get());

	componentMgr->removeComponent(transformComponent->entityID, &typeid(TransformComponent));
}

