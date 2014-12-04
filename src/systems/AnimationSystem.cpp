#include "AnimationSystem.h"

void AnimationSystem::start()
{
	safeAddListener(dynamic_cast<IEventListener*>(this), EventType("ComponentAdded"));
	safeAddListener(dynamic_cast<IEventListener*>(this), EventType("EntityDestroyed"));
	safeAddListener(dynamic_cast<IEventListener*>(this), EventType("RemoveComponent"));
	safeAddListener(dynamic_cast<IEventListener*>(this), EventType("ChangeAnimation"));
	LOG_DEBUG("Animation system started");
}

void AnimationSystem::update(float deltaTime)
{
	std::vector<std::shared_ptr<Component>> components = componentMgr->getComponentsByType(&typeid(AnimationComponent));

	for (std::shared_ptr<Component> component : components)
	{
		if (dynamic_cast<AnimationComponent*>(component.get())->animation)
			dynamic_cast<AnimationComponent*>(component.get())->animation->addTime(deltaTime);
	}
}

void AnimationSystem::stop()
{
	safeDelListener(dynamic_cast<IEventListener*>(this), EventType("ComponentAdded"));
	safeDelListener(dynamic_cast<IEventListener*>(this), EventType("EntityDestroyed"));
	safeDelListener(dynamic_cast<IEventListener*>(this), EventType("RemoveComponent"));
	safeDelListener(dynamic_cast<IEventListener*>(this), EventType("ChangeAnimation"));
	LOG_DEBUG("Animation system stopped");
}

bool AnimationSystem::handleEvent(const GameEvent& event)
{
	if (event.getEventType() == EventType("ComponentAdded"))
	{
		const ComponentEvent& componentEvent = dynamic_cast<const ComponentEvent&>(event);
		if (componentEvent.componentType == &typeid(AnimationComponent))
			initAnimationComponent(componentMgr->getComponent(componentEvent.entityID, &typeid(AnimationComponent)));
	}
	else if (event.getEventType() == EventType("ChangeAnimation"))
	{
		const AnimationEvent& animationEvent = dynamic_cast<const AnimationEvent&>(event);
		changeAnimation(animationEvent.entityID, animationEvent.animationName);
	}
	else if (event.getEventType() == EventType("EntityDestroyed"))
	{
		const EntityEvent& entityEvent = dynamic_cast<const EntityEvent&>(event);
		if (componentMgr->getComponent(entityEvent.entityID, &typeid(AnimationComponent)))
			removeAnimationComponent(componentMgr->getComponent(entityEvent.entityID, &typeid(AnimationComponent)));
	}
	else if (event.getEventType() == EventType("RemoveComponent"))
	{
		const ComponentEvent& componentEvent = dynamic_cast<const ComponentEvent&>(event);
		if (componentEvent.componentType == &typeid(AnimationComponent))
			removeAnimationComponent(componentMgr->getComponent(componentEvent.entityID, &typeid(AnimationComponent)));
	}

	return true;
}

void AnimationSystem::initAnimationComponent(std::shared_ptr<Component> component)
{
	AnimationComponent* animationComponent = dynamic_cast<AnimationComponent*>(component.get());
	Ogre::Entity* entity = dynamic_cast<MeshComponent*>(componentMgr->getComponent(animationComponent->entityID, &typeid(MeshComponent)).get())->entity;
	animationComponent->animation = entity->getAnimationState(animationComponent->animationName);
	animationComponent->animation->setLoop(true);
	animationComponent->animation->setEnabled(true);
}

void AnimationSystem::removeAnimationComponent(std::shared_ptr<Component> component)
{
	AnimationComponent* animationComponent = dynamic_cast<AnimationComponent*>(component.get());
	animationComponent->animation->setEnabled(false);
	sceneMgr->destroyAnimationState(animationComponent->animationName);
	animationComponent->animation = 0;

	componentMgr->removeComponent(animationComponent->entityID, &typeid(AnimationComponent));
}

void AnimationSystem::changeAnimation(int objectID, std::string animationName)
{
	AnimationComponent* animationComponent = dynamic_cast<AnimationComponent*>(componentMgr->getComponent(objectID, &typeid(AnimationComponent)).get());
	Ogre::Entity* entity = dynamic_cast<MeshComponent*>(componentMgr->getComponent(objectID, &typeid(MeshComponent)).get())->entity;
	animationComponent->animation = entity->getAnimationState(animationName);
	animationComponent->animation->setLoop(true);
	animationComponent->animation->setEnabled(true);
	entity->getSkeleton()->setBlendMode(Ogre::ANIMBLEND_CUMULATIVE);
}
