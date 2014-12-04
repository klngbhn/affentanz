#include "EventLogger.h"

EventLogger::EventLogger()
{

}

EventLogger::~EventLogger() 
{

}

bool EventLogger::handleEvent(const GameEvent& event)
{
	LOG_DEBUG("Event: " + engine_functions::to_string(event.getEventType().getHashedString()) + " " + event.getEventType().getKeyString());

	return true;
}
