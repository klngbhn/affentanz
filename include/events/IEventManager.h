#pragma once

#include "OgrePlatform.h"
#include <assert.h>
#include <memory>
#include <string>

#include "IEventListener.h"

typedef std::shared_ptr<IEventListener> EventListenerPtr;

/*
 * This is the guideline for the event manager.
 */
class IEventManager
{
public:
	enum eConstants { INFINITE = 0xffffffff };
	explicit IEventManager(const std::string name, bool setAsGlobal);
	virtual ~IEventManager();

	virtual bool addListener(IEventListener* inHandler, const EventType& inType) = 0;
	virtual bool delListener(IEventListener* inHandler, const EventType& inType) = 0;
	virtual bool queueEvent(const GameEventPtr& inEvent) = 0;
	virtual bool abortEvent(const EventType& inType, bool allOfType = false) = 0;
	virtual bool tick(unsigned long maxMillis = INFINITE) = 0;
	virtual bool validateType(const EventType& inType) const = 0;

private:
	static IEventManager* getInstance();

	friend bool safeAddListener(IEventListener* inHandler, const EventType& inType);
	friend bool safeDelListener(IEventListener* inHandler, const EventType& inType);
	friend bool safeQueueEvent(const GameEventPtr& inEvent);
    #if OGRE_PLATFORM == OGRE_PLATFORM_APPLE_IOS
	friend bool safeAbortEvent(const EventType& inType, bool allOfType);
	friend bool safeTickEventManager(unsigned long maxMillis);
    #else
    friend bool safeAbortEvent(const EventType& inType, bool allOfType = false);
	friend bool safeTickEventManager(unsigned long maxMillis = IEventManager::INFINITE);
    #endif
	friend bool safeValidateEventType(const EventType& inType);
};

static IEventManager* instance = NULL;

bool safeAddListener(IEventListener* inHandler, const EventType& inType);
bool safeDelListener(IEventListener* inHandler, const EventType& inType);
bool safeQueueEvent(const GameEventPtr& inEvent);
#if OGRE_PLATFORM == OGRE_PLATFORM_APPLE_IOS
bool safeAbortEvent(const EventType& inType, bool allOfType = false);
bool safeTickEventManager(unsigned long maxMillis = IEventManager::INFINITE);
#else
bool safeAbortEvent(const EventType& inType, bool allOfType);
bool safeTickEventManager(unsigned long maxMillis);
#endif
bool safeValidateEventType(const EventType& inType);
