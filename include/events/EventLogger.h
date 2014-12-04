#pragma once

#include "IEventListener.h"
#include "Log.h"
#include "Functions.h"

/*
 * This class logs all executed events to the command-line output and the logfile.
 */
class EventLogger : public IEventListener
{
public:
	explicit EventLogger();
	~EventLogger();
	std::string getName() { return "EventLogger"; }
	bool handleEvent(const GameEvent& event);
};

