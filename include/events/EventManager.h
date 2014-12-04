#pragma once

#include <vector>
#include <set>
#include <list>
#include <map>
#include <cstring>
#include <OgreRoot.h>

#include "IEventManager.h"
#include "GameEvent.h"
#include "Log.h"
#include "Functions.h"

typedef std::vector<IEventListener*> EventListenerList;
typedef std::vector<EventType> EventTypeList;

/*
 * The event manager is the central place for all events. It can add and delete event listener
 * for specific events, add new events (in the event queue) and execute them (inform all
 * listeners for that event).
 * The event manager is based mostly on the example from "Game Coding Complete" (but it's not
 * copy-pasted).
 */
class EventManager : public IEventManager
{
public:
	explicit EventManager(const std::string name, bool setAsGlobal);
	~EventManager();

	bool addListener(IEventListener* inListener, const EventType& inType);
	bool delListener(IEventListener* inListener, const EventType& inType);
	bool queueEvent(const GameEventPtr& inEvent);
	bool abortEvent(const EventType& inType, bool allOfType);
	bool tick(unsigned long maxMillis);
	bool validateType(const EventType& inType) const;

	EventListenerList getListenerList(const EventType& eventType) const;
	EventTypeList getTypeList() const;

private:
	typedef std::set<EventType> EventTypeSet;
	typedef std::pair<typename EventTypeSet::iterator, bool> EventTypeSetInsertResult;
	typedef std::list<IEventListener*> EventListenerTable;
	typedef std::map<unsigned int, EventListenerTable> EventListenerMap;
	typedef std::pair<unsigned int, EventListenerTable> EventListenerMapEntry;
	typedef std::pair<typename EventListenerMap::iterator, bool> EventListenerMapInsertResult;
	typedef std::list<GameEventPtr> EventQueue;

	enum eConstants { numberQueues = 2 };

	EventTypeSet typeList;
	EventListenerMap registry;
	EventQueue queues[numberQueues];
	int activeQueue;
};
