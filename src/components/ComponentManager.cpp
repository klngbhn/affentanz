#include "ComponentManager.h"

ComponentManager::ComponentManager()
	:nextEntityID(0)
{
}

ComponentManager::~ComponentManager()
{
}

int ComponentManager::createEntity()
{
	return createEntity(engine_functions::to_string(nextEntityID));
}

int ComponentManager::createEntity(std::string entityName)
{
	int entityID = nextEntityID;
	nextEntityID++;
	entities[entityName] = entityID;
	entitiesByID[entityID] = entityName;
	GameEventPtr event(new EntityEvent("EntityCreated", entityID));
	safeQueueEvent(event);
	return entityID;
}

int ComponentManager::getEntity(std::string entityName)
{
	return entities[entityName];
}

void ComponentManager::destroyEntity(std::string entityName)
{
	destroyEntity(entities[entityName]);
}

void ComponentManager::destroyEntity(int entityID)
{
	if (entitiesByID.find(entityID) != entitiesByID.end())
	{
		std::string entityName = entitiesByID[entityID];
		entities.erase(entityName);
		entitiesByID.erase(entityID);
		GameEventPtr event(new EntityEvent("EntityDestroyed", entityID));
		safeQueueEvent(event);
	}
}

void ComponentManager::addComponent(int entityID, Component& component)
{
	component.entityID = entityID;
	std::shared_ptr<Component> comp = std::shared_ptr<Component>(&component);
	components[entityID].insert(std::make_pair(&typeid(component), comp));
	componentsByType[&typeid(component)].push_back(comp);
	componentsByEntity[entityID].push_back(comp);
	GameEventPtr event(new ComponentEvent("ComponentAdded", entityID, &typeid(component)));
	safeQueueEvent(event);
}

std::shared_ptr<Component> ComponentManager::getComponent(int entityID, const std::type_info* componentType)
{
	if (components.find(entityID) != components.end())
	{
		std::map<const std::type_info*, std::shared_ptr<Component>> typeMap = components[entityID];
		if (typeMap.find(componentType) != typeMap.end())
			return components[entityID].at(componentType);
	}
	return NULL;
}

std::vector<std::shared_ptr<Component>> ComponentManager::getComponentsByType(const std::type_info* componentType)
{
	return componentsByType[componentType];
}

std::vector<std::shared_ptr<Component>> ComponentManager::getComponentsByEntity(int entityID)
{
	return componentsByEntity[entityID];
}

void ComponentManager::removeComponent(int entityID, const std::type_info* componentType)
{
	if (components.find(entityID) != components.end())
	{
		std::map<const std::type_info*, std::shared_ptr<Component>> typeMap = components[entityID];
		typeMap.erase(componentType);
	}
	for (int i = 0; i++; i < componentsByType[componentType].size())
	{
		if (componentsByType[componentType].at(i)->entityID == entityID)
		{
			std::vector<std::shared_ptr<Component>> compVec = componentsByType[componentType];
			compVec.erase(compVec.begin() + i);
		}
	}
	for (int i = 0; i++; i < componentsByEntity[entityID].size())
	{
		if (&typeid(componentsByEntity[entityID].at(i).get()) == componentType)
			componentsByEntity[entityID].erase(componentsByEntity[entityID].begin() + i);
	}
	//componentsByType.erase(componentType);
	//componentsByEntity.erase(entityID);
	GameEventPtr event(new ComponentEvent("ComponentRemoved", entityID, componentType));
	safeQueueEvent(event);
}
