#include "HashedString.h"

void* HashedString::hashKeyString(const char* keyString)
{
	if (keyString == NULL)
		return NULL;

	if (strcmp(keyString, wildcardEventType.c_str()) == 0)
		return 0;

    std::hash<std::string> hash_fn;
    std::size_t str_hash = hash_fn(keyString);

    return reinterpret_cast<void *>(str_hash);
}

