#pragma once

#include <OgreRoot.h>
#if OGRE_PLATFORM == OGRE_PLATFORM_APPLE_IOS
#else
#include "MyGUI.h"
#endif

#include "ConfigLoader.h"

#if OGRE_PLATFORM == OGRE_PLATFORM_ANDROID
	#include <android/log.h>
#endif

/*
 * This file wraps a common logging system around the different logs on the different platforms.
 * On desktop platforms, the ogre log manager is used. Android and ios have their own log mechanisms.
 */

#if OGRE_PLATFORM == OGRE_PLATFORM_ANDROID

	#define LOG_INFO(s) if (ConfigLoader::currentLogLevel <= INFO) ((void)__android_log_print(ANDROID_LOG_INFO, "[INFO] ", std::string(s).c_str())); //__VA_ARGS__
	#define LOG_WARN(s) if (ConfigLoader::currentLogLevel <= WARN) ((void)__android_log_print(ANDROID_LOG_INFO, "[WARN] ", std::string(s).c_str()));
	#define LOG_DEBUG(s) if (ConfigLoader::currentLogLevel <= DEBUGL) ((void)__android_log_print(ANDROID_LOG_INFO, "[DEBUG] ", std::string(s).c_str()));
	#define LOG_ERROR(s) if (ConfigLoader::currentLogLevel <= ERROR) ((void)__android_log_print(ANDROID_LOG_INFO, "[ERROR] ", std::string(s).c_str()));

#elif OGRE_PLATFORM == OGRE_PLATFORM_LINUX

	#define LOG_INFO(s) if (ConfigLoader::currentLogLevel <= INFO) { Ogre::LogManager::getSingleton().getDefaultLog()->logMessage(std::string("[INFO] ") + s); if (MyGUI::Gui::getInstancePtr() && MyGUI::Gui::getInstancePtr()->findWidget<MyGUI::EditBox>("Console", false)) { MyGUI::EditBox* edit = MyGUI::Gui::getInstancePtr()->findWidget<MyGUI::EditBox>("Console"); if(edit->getTextLength() > 1000) edit->eraseText(0, std::string(edit->getCaption()).find_first_of("\\n")); edit->setCaptionWithReplacing(edit->getCaption()+"#ffffff" + std::string("[INFO] ") + s + "\\n"); } }
	#define LOG_WARN(s) if (ConfigLoader::currentLogLevel <= WARN) { Ogre::LogManager::getSingleton().getDefaultLog()->logMessage(std::string("[WARN] ") + s); if (MyGUI::Gui::getInstancePtr() && MyGUI::Gui::getInstancePtr()->findWidget<MyGUI::EditBox>("Console", false)) { MyGUI::EditBox* edit = MyGUI::Gui::getInstancePtr()->findWidget<MyGUI::EditBox>("Console"); if(edit->getTextLength() > 1000) edit->eraseText(0, std::string(edit->getCaption()).find_first_of("\\n")); edit->setCaptionWithReplacing(edit->getCaption()+"#ffff00" + std::string("[WARN] ") + s + "\\n"); } }
	#define LOG_DEBUG(s) if (ConfigLoader::currentLogLevel <= DEBUGL) { Ogre::LogManager::getSingleton().getDefaultLog()->logMessage(std::string("[DEBUG] ") + s); if (MyGUI::Gui::getInstancePtr() && MyGUI::Gui::getInstancePtr()->findWidget<MyGUI::EditBox>("Console", false)) { MyGUI::EditBox* edit = MyGUI::Gui::getInstancePtr()->findWidget<MyGUI::EditBox>("Console"); if(edit->getTextLength() > 1000) edit->eraseText(0, std::string(edit->getCaption()).find_first_of("\\n")); edit->setCaptionWithReplacing(edit->getCaption()+"#0000ff" + std::string("[DEBUG] ") + s + "\\n"); } }
	#define LOG_ERROR(s) if (ConfigLoader::currentLogLevel <= ERROR) { Ogre::LogManager::getSingleton().getDefaultLog()->logMessage(std::string("[ERROR] ") + s); if (MyGUI::Gui::getInstancePtr() && MyGUI::Gui::getInstancePtr()->findWidget<MyGUI::EditBox>("Console", false)) { MyGUI::EditBox* edit = MyGUI::Gui::getInstancePtr()->findWidget<MyGUI::EditBox>("Console"); if(edit->getTextLength() > 1000) edit->eraseText(0, std::string(edit->getCaption()).find_first_of("\\n")); edit->setCaptionWithReplacing(edit->getCaption()+"#ff0000" + std::string("[ERROR] ") + s + "\\n"); } }

#else

    #define LOG(s)
    #define LOG_INFO(s) if (ConfigLoader::currentLogLevel <= INFO) { Ogre::LogManager::getSingleton().getDefaultLog()->logMessage(std::string("[INFO] ") + s); }
    #define LOG_WARN(s) if (ConfigLoader::currentLogLevel <= WARN) { Ogre::LogManager::getSingleton().getDefaultLog()->logMessage(std::string("[WARN] ") + s); }
	#define LOG_DEBUG(s) if (ConfigLoader::currentLogLevel <= DEBUGL) { Ogre::LogManager::getSingleton().getDefaultLog()->logMessage(std::string("[DEBUG] ") + s); }
    #define LOG_ERROR(s) if (ConfigLoader::currentLogLevel <= ERROR) { Ogre::LogManager::getSingleton().getDefaultLog()->logMessage(std::string("[ERROR] ") + s); }

#endif
