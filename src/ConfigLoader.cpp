#include "ConfigLoader.h"

ConfigLoader::ConfigLoader(std::string pathname)
{
	this->pathname = pathname;
}

ConfigLoader::~ConfigLoader()
{

}

#if OGRE_PLATFORM == OGRE_PLATFORM_ANDROID
bool ConfigLoader::readConfig(struct android_app* state)
#else
bool ConfigLoader::readConfig()
#endif
{
	Ogre::ConfigFile file;

#if OGRE_PLATFORM == OGRE_PLATFORM_ANDROID
	this->state = state;
    file.load(openAPKFile(pathname));
#elif OGRE_PLATFORM == OGRE_PLATFORM_APPLE_IOS
    file.load(Ogre::macBundlePath() + "/" + pathname);
#else
    file.load("assets/" + pathname);
#endif

	Ogre::ConfigFile::SectionIterator seci = file.getSectionIterator();
	while (seci.hasMoreElements())
	{
		Ogre::String section, key, value;
		section = seci.peekNextKey();
		Ogre::ConfigFile::SettingsMultiMap* settings = seci.getNext();
		Ogre::ConfigFile::SettingsMultiMap::iterator i;

		for (i = settings->begin(); i != settings->end(); i++)
		{
			key = i->first;
			value = i->second;
			if (section == "assets")
				resources[key] = value;
			if (section == "plugins")
				plugins.push_back(key);
			else
				options[key] = value;
		}
	}

    // Check if log level is set
    if (options["logLevel"].compare("debug") == 0)
    	currentLogLevel = DEBUGL;
    else if (options["logLevel"].compare("warn") == 0)
        currentLogLevel = WARN;
    else if (options["logLevel"].compare("error") == 0)
        currentLogLevel = ERROR;
    else if (options["logLevel"].compare("info") == 0)
        currentLogLevel = INFO;
    else
    	currentLogLevel = INFO;

    // Check if app name is set
    if (options["appName"].empty())
    	options["appName"] = "engine";

    // Check if resolution is set
	if (options["resolution"].empty())
		options["resolution"] = "800x600";

	// Check if fullscreen is set
	if (options["fullscreen"].empty() || (options["fullscreen"] != "true" && options["fullscreen"] != "false"))
		options["fullscreen"] = "false";

	// Check if anti-aliasing is set
	if (options["FSAA"].empty())
		options["FSAA"] = "0";

	// Check if colour depth is set
	if (options["colourDepth"].empty())
		options["colourDepth"] = "32";

	// Check if vsync is set
	if (options["vsync"].empty())
		options["vsync"] = "false";

	// Check if vsync is set
	if (options["resourceLoading"].empty() || (options["resourceLoading"] != "fileSystem" && options["resourceLoading"] != "zip"))
		options["resourceLoading"] = "zip";

    return true;
}

#if OGRE_PLATFORM == OGRE_PLATFORM_ANDROID
Ogre::DataStreamPtr ConfigLoader::openAPKFile(const Ogre::String& fileName)
{
	Ogre::DataStreamPtr stream;
	const char * c = fileName.c_str();
    AAsset* asset = AAssetManager_open(state->activity->assetManager, fileName.c_str(), AASSET_MODE_BUFFER);
    if(asset)
    {
		off_t length = AAsset_getLength(asset);
        void* membuf = OGRE_MALLOC(length, Ogre::MEMCATEGORY_GENERAL);
        memcpy(membuf, AAsset_getBuffer(asset), length);
        AAsset_close(asset);

        stream = Ogre::DataStreamPtr(new Ogre::MemoryDataStream(membuf, length, true, true));
    }
    return stream;
}
#endif

LogLevel ConfigLoader::currentLogLevel;
