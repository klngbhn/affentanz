#pragma once

#include <typeinfo>
#include <memory>

#include "Component.h"
#include "IEventManager.h"
#include "Functions.h"

/*
 * The ComponentManager keeps references to all components of a scene and
 * provides methods to add, get and remove a component, but also to create
 * and destroy entities.
 */
class ComponentManager
{
public:
	ComponentManager();
	virtual ~ComponentManager();
	int createEntity();
	int createEntity(std::string entityName);
	int getEntity(std::string entityName);
	void destroyEntity(std::string entityName);
	void destroyEntity(int entityID);
	void addComponent(int entityID, Component& component);
	std::shared_ptr<Component> getComponent(int entityID, const std::type_info* componentType);
	std::vector<std::shared_ptr<Component>> getComponentsByType(const std::type_info* componentType);
	std::vector<std::shared_ptr<Component>> getComponentsByEntity(int entityID);
	void removeComponent(int entityID, const std::type_info* componentType);
private:
	int nextEntityID;
	std::map<std::string, int> entities;
	std::map<int, std::string> entitiesByID;
	std::map<int, std::map<const std::type_info*, std::shared_ptr<Component>>> components;
	std::map<const std::type_info*, std::vector<std::shared_ptr<Component>>> componentsByType;
	std::map<int, std::vector<std::shared_ptr<Component>>> componentsByEntity;
};
