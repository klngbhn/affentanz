#include "MeshSystem.h"

void MeshSystem::start()
{
	safeAddListener(dynamic_cast<IEventListener*>(this), EventType("ComponentAdded"));
	safeAddListener(dynamic_cast<IEventListener*>(this), EventType("EntityDestroyed"));
	safeAddListener(dynamic_cast<IEventListener*>(this), EventType("RemoveComponent"));
	safeAddListener(dynamic_cast<IEventListener*>(this), EventType("ChangeMaterialDiffuseColor"));
	LOG_DEBUG("Mesh system started");
}

void MeshSystem::update(float deltaTime)
{

}

void MeshSystem::stop()
{
	safeDelListener(dynamic_cast<IEventListener*>(this), EventType("ComponentAdded"));
	safeDelListener(dynamic_cast<IEventListener*>(this), EventType("EntityDestroyed"));
	safeDelListener(dynamic_cast<IEventListener*>(this), EventType("RemoveComponent"));
	safeDelListener(dynamic_cast<IEventListener*>(this), EventType("ChangeMaterialDiffuseColor"));
	LOG_DEBUG("Mesh system stopped");
}

bool MeshSystem::handleEvent(const GameEvent& event)
{
	if (event.getEventType() == EventType("ComponentAdded"))
	{
		const ComponentEvent& componentEvent = dynamic_cast<const ComponentEvent&>(event);
		if (componentEvent.componentType == &typeid(MeshComponent))
			initMeshComponent(componentMgr->getComponent(componentEvent.entityID, &typeid(MeshComponent)));
	}
	else if (event.getEventType() == EventType("EntityDestroyed"))
	{
		const EntityEvent& entityEvent = dynamic_cast<const EntityEvent&>(event);
		if (componentMgr->getComponent(entityEvent.entityID, &typeid(MeshComponent)))
			removeMeshComponent(componentMgr->getComponent(entityEvent.entityID, &typeid(MeshComponent)));
	}
	else if (event.getEventType() == EventType("RemoveComponent"))
	{
		const ComponentEvent& componentEvent = dynamic_cast<const ComponentEvent&>(event);
		if (componentEvent.componentType == &typeid(MeshComponent))
			removeMeshComponent(componentMgr->getComponent(componentEvent.entityID, &typeid(MeshComponent)));
	}
	else if (event.getEventType() == EventType("ChangeMaterialDiffuseColor"))
	{
		const ColorEvent& castedEvent = dynamic_cast<const ColorEvent&>(event);
		Ogre::MaterialManager::getSingleton().getByName(castedEvent.materialName)->setDiffuse(castedEvent.r, castedEvent.g, castedEvent.b, castedEvent.a);
	}

	return true;
}

void MeshSystem::initMeshComponent(std::shared_ptr<Component> component)
{
	MeshComponent* meshComponent = dynamic_cast<MeshComponent*>(component.get());

	if (meshComponent->isPlane)
	{
		Ogre::Plane plane(meshComponent->normal, meshComponent->distance);
		Ogre::MeshManager::getSingleton().createPlane(meshComponent->meshName, Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
			 plane, meshComponent->width, meshComponent->height, meshComponent->xSegs, meshComponent->ySegs, meshComponent->hasNormals, meshComponent->numTexCoordSets, meshComponent->uTile, meshComponent->vTile, meshComponent->upVector);
	}

	meshComponent->entity = sceneMgr->createEntity(meshComponent->entityName, meshComponent->meshName);
	meshComponent->entity->setCastShadows(meshComponent->castShadows);

	if (meshComponent->isPlane)
	{
		Ogre::MaterialPtr material = Ogre::MaterialManager::getSingleton().getByName(meshComponent->material);
		meshComponent->entity->setMaterial(material);
	}
}

void MeshSystem::removeMeshComponent(std::shared_ptr<Component> component)
{
	MeshComponent* meshComponent = dynamic_cast<MeshComponent*>(component.get());

	sceneMgr->destroyEntity(meshComponent->entity);
	meshComponent->entity = 0;

	componentMgr->removeComponent(meshComponent->entityID, &typeid(MeshComponent));
}
