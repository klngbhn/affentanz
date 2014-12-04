#pragma once

#include <stdlib.h>
#include <cstring>
#include <functional>
#include <string>
#include <iostream>

const std::string wildcardEventType = "*";

/*
 * This class represents a hashed string, e.g. for an event type.
 * It stores the original key as a string and the hashed value as integer
 * and it has a function to hash strings.
 */
class HashedString
{
public:
	explicit HashedString(const std::string key)
		: hashedString(hashKeyString(key.c_str())),
		  keyString(key)
	{}

	unsigned long getHashedString() const
	{
		return reinterpret_cast<unsigned long>(hashedString);
	}

	const std::string getKeyString() const { return keyString; }

	bool operator< (HashedString const & o) const
	{
		bool result = (getHashedString() < o.getHashedString());
		return result;
	}

	bool operator== (HashedString const & o) const
	{
		bool result = (getHashedString() == o.getHashedString());
		return result;
	}

	static void* hashKeyString(const char* keyString);

private:
	void* hashedString;
	std::string keyString;
};
