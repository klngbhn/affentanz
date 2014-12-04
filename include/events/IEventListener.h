#pragma once

#include <string>
#include "GameEvent.h"

/*
 * This interface should be implemented when a class has to
 * listen to one or more events.
 */
class IEventListener
{
public:
	explicit IEventListener() {}
	virtual ~IEventListener() {}

	virtual std::string getName() = 0;

	virtual bool handleEvent(const GameEvent& event) = 0;
};

