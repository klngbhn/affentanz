#include "EventManager.h"

EventManager::EventManager(const std::string name, bool setAsGlobal)
	: IEventManager(name, setAsGlobal),
	  activeQueue(0)
{
}

EventManager::~EventManager() 
{
	activeQueue = 0;
}

bool EventManager::addListener(IEventListener* inListener, const EventType& inType)
{
	//if (!validateType(inType))
		//return false;

	EventTypeSet::iterator eventIterator = typeList.find(inType);
	EventListenerMap::iterator eventListenerMapIterator = registry.find(inType.getHashedString());
	
	if (eventListenerMapIterator == registry.end())
	{
		EventListenerMapInsertResult eventListenerMapInsertResult = registry.insert(EventListenerMapEntry(inType.getHashedString(), EventListenerTable()));
		
		if (eventListenerMapInsertResult.second == false)
			return false;
		
		if (eventListenerMapInsertResult.first == registry.end())
			return false;
		
		eventListenerMapIterator = eventListenerMapInsertResult.first;
	}

	EventListenerTable &eventListenerTable = (*eventListenerMapIterator).second;
	
	for (EventListenerTable::iterator iterator = eventListenerTable.begin(), iteratorEnd = eventListenerTable.end(); iterator != iteratorEnd; iterator++)
	{
		bool listenerMatch = (*iterator == inListener);
		
		if (listenerMatch)
			return false;
	}

	eventListenerTable.push_back(inListener);
	return true;
}

bool EventManager::delListener(IEventListener* inListener, const EventType& inType)
{
//	if (!validateType(inType))
//		return false;

	bool returnCode = false;
	
	for (EventListenerMap::iterator iterator = registry.begin(), iteratorEnd = registry.end(); iterator != iteratorEnd; iterator++)
	{
		unsigned const int kEventId = iterator->first;
		EventListenerTable &table = iterator->second;
		
		for (EventListenerTable::iterator iterator2 = table.begin(), iterator2End = table.end(); iterator2 != iterator2End; iterator2++)
		{
			if (*iterator2 == inListener)
			{
				table.erase(iterator2);
				
				returnCode = true;
				
				break;
			}
		}
	}
	return returnCode;
}

bool EventManager::queueEvent(const GameEventPtr& inEvent)
{
	assert(activeQueue >= 0);
	assert(activeQueue < numberQueues);
	
	if (!validateType(inEvent->getEventType()))
		return false;
	
	EventListenerMap::const_iterator iterator = registry.find(inEvent->getEventType().getHashedString());
	
	if (iterator == registry.end())
	{
		EventListenerMap::const_iterator iteratorWC = registry.find(0);
		
		if (iteratorWC == registry.end())
			return false;
	}
	
	queues[activeQueue].push_back(inEvent);
	
	return true;
}

bool EventManager::abortEvent(const EventType& inType, bool allOfType)
{
	assert(activeQueue >= 0);
	assert(activeQueue < numberQueues);
	
	if (!validateType(inType))
		return false;
	
	EventListenerMap::iterator iterator = registry.find(inType.getHashedString());
	
	if (iterator == registry.end())
		return false;
	
	bool returnCode = false;
	
	EventQueue &eventQueue = queues[activeQueue];
	
	for (EventQueue::iterator iterator = eventQueue.begin(), iteratorEnd = eventQueue.end(); iterator != iteratorEnd; iterator++)
	{
		if ((*iterator)->getEventType() == inType)
		{
			iterator = eventQueue.erase(iterator);
			returnCode = true;
			if (!allOfType)
				break;
		}
		else
		{
			++iterator;
		}
	}
	
	return returnCode;
}

bool EventManager::tick(unsigned long maxMillis)
{
	unsigned long currentMs = Ogre::Root::getSingleton().getTimer()->getMilliseconds();
	unsigned long maxMs = (maxMillis == IEventManager::INFINITE) ? IEventManager::INFINITE : (currentMs + maxMillis);
	
	EventListenerMap::const_iterator iteratorWildcardListeners = registry.find(0);
	
	int queueToProcess = activeQueue;
	activeQueue = (activeQueue + 1) % numberQueues;
	queues[activeQueue].clear();

	// Go through all events in the active queue
	while (queues[queueToProcess].size() > 0)
	{
		GameEventPtr event = queues[queueToProcess].front();
		queues[queueToProcess].pop_front();
		
		// Iterate through all wildcard (=listens to all events) event listeners
		if (iteratorWildcardListeners != registry.end())
		{
			const EventListenerTable &table = iteratorWildcardListeners->second;
			
			for (IEventListener* eventListener : table)
			{
				eventListener->handleEvent(*event);
			}
		}

		const EventType &eventType = event->getEventType();
		EventListenerMap::const_iterator iteratorListeners = registry.find(eventType.getHashedString());

		// Now iterate through all event listeners for this event type
		if (iteratorListeners != registry.end())
		{
			const EventListenerTable &table = iteratorListeners->second;

			for (IEventListener* eventListener : table)
			{
				eventListener->handleEvent(*event);
			}
		}

		currentMs = Ogre::Root::getSingleton().getTimer()->getMilliseconds();
		
		// Check if time limit is achieved
		if (maxMillis != IEventManager::INFINITE)
		{
			if (currentMs >= maxMs)
				break;
		}

		event.reset();
	}
	
	bool queueFlushed = (queues[queueToProcess].size() == 0);
	
	// Copy events, that wasn't handled, to the other queue to proces them next frame
	if (!queueFlushed)
	{
		while (queues[queueToProcess].size() > 0)
		{
			GameEventPtr event = queues[queueToProcess].back();
			queues[queueToProcess].pop_back();
			queues[activeQueue].push_front(event);
		}
	}
	
	return queueFlushed;
}

bool EventManager::validateType(const EventType& inType) const
{
	if (0 == inType.getKeyString().length())
		return false;
	
	if ((inType.getHashedString() == 0) && (inType.getKeyString().compare(wildcardEventType) != 0))
		return false;
	
	EventTypeSet::const_iterator eventIterator = typeList.find(inType);
	
	if (eventIterator == typeList.end())
	{
		//assert(0 && "Failed event type validation - it was probably not registered with the Event Manager!");
		//return false;
	}
	
	return true;
}


EventListenerList EventManager::getListenerList(const EventType& eventType) const
{
	if (!validateType(eventType))
		return EventListenerList();
	
	EventListenerMap::const_iterator iteratorListeners = registry.find(eventType.getHashedString());
	
	if (iteratorListeners == registry.end())
		return EventListenerList();
	
	const EventListenerTable &table = iteratorListeners->second;
	
	if (table.size() == 0)
		return EventListenerList();
	
	EventListenerList result;
	result.reserve(table.size());
	
	for (EventListenerTable::const_iterator iterator = table.begin(), end = table.end(); iterator != end; iterator++)
	{
		result.push_back(*iterator);
	}
	
	return result;
}

EventTypeList EventManager::getTypeList() const
{
	if (typeList.size() == 0)
		return EventTypeList();
	
	EventTypeList result;
	result.reserve(typeList.size());
	
	for (EventTypeSet::const_iterator iterator = typeList.begin(), iteratorEnd = typeList.end(); iterator != iteratorEnd; iterator++)
	{
		result.push_back(*iterator);
	}
	
	return result;
}


IEventManager* IEventManager::getInstance()
{
	return instance;
}

IEventManager::IEventManager(const std::string name, bool setAsGlobal)
{
	if (setAsGlobal)
		instance = this;
}

IEventManager::~IEventManager()
{
	if (instance == this)
		instance = NULL;
}


//TODO: Error checking & exceptions, serialization of events

bool safeAddListener(IEventListener* inHandler, const EventType& inType)
{
	assert(IEventManager::getInstance() && "No event manager found"); //TODO output for developer: No event manager!
	return IEventManager::getInstance()->addListener(inHandler, inType);
}

bool safeDelListener(IEventListener* inHandler, const EventType& inType)
{
	assert(IEventManager::getInstance()); //TODO output for developer: No event manager!
	return IEventManager::getInstance()->delListener(inHandler, inType);
}

bool safeQueueEvent(const GameEventPtr& inEvent)
{
	assert(IEventManager::getInstance()); //TODO output for developer: No event manager!
	return IEventManager::getInstance()->queueEvent(inEvent);
}

bool safeAbortEvent(const EventType& inType, bool allOfType)
{
	assert(IEventManager::getInstance()); //TODO output for developer: No event manager!
	return IEventManager::getInstance()->abortEvent(inType, allOfType);
}

bool safeTickEventManager(unsigned long maxMillis /*= IEventManager::kINFINITE*/)
{
	assert(IEventManager::getInstance()); //TODO output for developer: No event manager!
	return IEventManager::getInstance()->tick(maxMillis);
}

bool safeValidateEventType(const EventType& inType)
{
	assert(IEventManager::getInstance()); //TODO output for developer: No event manager!
	return IEventManager::getInstance()->validateType(inType);
}

