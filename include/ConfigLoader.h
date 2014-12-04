#pragma once

#include <Ogre.h>

#if OGRE_PLATFORM == OGRE_PLATFORM_ANDROID
	#include <android_native_app_glue.h>

	#include "Android/OgreAPKFileSystemArchive.h"
	#include "Android/OgreAPKZipArchive.h"
#elif OGRE_PLATFORM == OGRE_PLATFORM_APPLE_IOS
    #include <iOS/macUtils.h>
#endif

/*
 * There are four log Levels: Debug, Warn, Error and Info.
 */
enum LogLevel
{
	DEBUGL,
	WARN,
	ERROR,
	INFO
};

/*
 * This class loads the config options from file, checks them for mistakes and
 * sorts them into different option containers.
 */
class ConfigLoader
{
public:
	ConfigLoader(std::string pathname);
	virtual ~ConfigLoader();
#if OGRE_PLATFORM == OGRE_PLATFORM_ANDROID
	bool readConfig(struct android_app* state);
#else
	bool readConfig();
#endif
	Ogre::NameValuePairList getOptions() { return options; }
	Ogre::NameValuePairList getScripts() { return scripts; }
	void setScripts (Ogre::NameValuePairList scripts) { this->scripts = scripts; }
	Ogre::NameValuePairList getResources() { return resources; }
	std::vector<std::string> getPlugins() { return plugins; }
	static LogLevel currentLogLevel;
private:
	Ogre::NameValuePairList options;
	Ogre::NameValuePairList scripts;
	Ogre::NameValuePairList resources;
	std::vector<std::string> plugins;
	std::string pathname;
#if OGRE_PLATFORM == OGRE_PLATFORM_ANDROID
	struct android_app* state;
	Ogre::DataStreamPtr openAPKFile(const Ogre::String& fileName);
#endif
};
