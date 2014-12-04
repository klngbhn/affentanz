#pragma once

#include <typeinfo>
#include <memory>

#include "HashedString.h"

class GameEvent;

typedef HashedString EventType;
typedef std::shared_ptr<GameEvent> GameEventPtr;

/*
 * This class represents a game event, that has an event type.
 */
class GameEvent
{
public:
	explicit GameEvent() {}
	virtual ~GameEvent() {}
	virtual const EventType& getEventType() const { return *eventType; }
	virtual const std::string getEventCategory() const { return eventCategory; }
protected:
	EventType* eventType;
	std::string eventCategory;
};

struct SystemEvent : public GameEvent
{
public:
	SystemEvent(std::string eventTypeName)
	{
		eventType = new EventType(eventTypeName);
		eventCategory = "systemevent";
	}
};

struct EntityEvent : public GameEvent
{
	EntityEvent(std::string eventTypeName, int entityID)
	{
		eventType = new EventType(eventTypeName);
		this->entityID = entityID;
		eventCategory = "entityevent";
	}

	int entityID;
};

struct ComponentEvent : public GameEvent
{
	ComponentEvent(std::string eventTypeName, int entityID, const std::type_info* componentType)
		:componentType(componentType)
	{
		eventType = new EventType(eventTypeName);
		this->entityID = entityID;
		eventCategory = "componentevent";
	}
	~ComponentEvent()
	{
	}

	int entityID;
	const std::type_info* componentType;
};

struct InputEvent : public GameEvent
{
	InputEvent(std::string eventTypeName, int entityID)
	{
		eventType = new EventType(eventTypeName);
		this->entityID = entityID;
		eventCategory = "inputevent";
		this->x = 0;
		this->y = 0;
	}

	InputEvent(std::string eventTypeName, int entityID, int x, int y)
	{
		eventType = new EventType(eventTypeName);
		this->entityID = entityID;
		eventCategory = "inputevent";
		this->x = x;
		this->y = y;
	}

	int entityID;
	int x;
	int y;
};

struct AnimationEvent : public GameEvent
{
	AnimationEvent(std::string eventTypeName, int entityID, std::string animationName)
		:animationName(animationName)
	{
		eventType = new EventType(eventTypeName);
		this->entityID = entityID;
		eventCategory = "animationevent";
	}

	int entityID;
	std::string animationName;
};

struct SceneEvent : public GameEvent
{
public:
	SceneEvent(std::string eventTypeName, std::string sceneName)
	{
		eventType = new EventType(eventTypeName);
		this->sceneName = sceneName;
		eventCategory = "sceneevent";
	}

	std::string sceneName;
};

struct GUIEvent : public GameEvent
{
public:
	GUIEvent(std::string eventTypeName, std::string widgetName, std::string value)
	{
		eventType = new EventType(eventTypeName);
		this->widgetName = widgetName;
		this->value = value;
		eventCategory = "guievent";
	}

	std::string widgetName;
	std::string value;
};

struct CollisionEvent : public GameEvent
{
public:
	CollisionEvent(std::string eventTypeName, int entityA, int entityB)
	{
		eventType = new EventType(eventTypeName);
		this->entityA = entityA;
		this->entityB = entityB;
		eventCategory = "collisionevent";
	}

	int entityA;
	int entityB;
};

struct ForceEvent : public GameEvent
{
public:
	ForceEvent(std::string eventTypeName, int entityID, float directionX, float directionY, float directionZ, float velocity)
	{
		eventType = new EventType(eventTypeName);
		this->entityID = entityID;
		this->directionX = directionX;
		this->directionY = directionY;
		this->directionZ = directionZ;
		this->velocity = velocity;
		eventCategory = "forceevent";
	}

	int entityID;
	float directionX;
	float directionY;
	float directionZ;
	float velocity;
};

struct ColorEvent : public GameEvent
{
public:
	ColorEvent(std::string eventTypeName, std::string materialName, float r, float g, float b, float a)
	{
		eventType = new EventType(eventTypeName);
		this->materialName = materialName;
		this->r = r;
		this->g = g;
		this->b = b;
		this->a = a;
		eventCategory = "colorevent";
	}

	std::string materialName;
	float r;
	float g;
	float b;
	float a;
};

struct FloatEvent : public GameEvent
{
public:
	FloatEvent(std::string eventTypeName, float value)
	{
		eventType = new EventType(eventTypeName);
		this->value = value;
	}

	float value;
};
