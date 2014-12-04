cmake_minimum_required(VERSION 2.6)

# INSERT NAME FOR BINARY HERE
set(APP_NAME engine)
 
project(${APP_NAME}_project)

include(CheckCXXCompilerFlag)
CHECK_CXX_COMPILER_FLAG("-std=c++11" COMPILER_SUPPORTS_CXX11)
CHECK_CXX_COMPILER_FLAG("-std=c++0x" COMPILER_SUPPORTS_CXX0X)
if(COMPILER_SUPPORTS_CXX11)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
elseif(COMPILER_SUPPORTS_CXX0X)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
else()
        message(STATUS "The compiler ${CMAKE_CXX_COMPILER} has no C++11 support. Please use a different C++ compiler.")
endif()
 
if(WIN32)
	set(CMAKE_MODULE_PATH "$ENV{OGRE_HOME}/CMake/;${CMAKE_MODULE_PATH}")
	set(OGRE_SAMPLES_INCLUDEPATH
		$ENV{OGRE_HOME}/Samples/include
	)
endif(WIN32)

if(UNIX)
	if(EXISTS "${CMAKE_SOURCE_DIR}/Dependencies/lib/OGRE/cmake")

	  set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/Dependencies/lib/OGRE/cmake/;${CMAKE_MODULE_PATH}")
	  set(OGRE_SAMPLES_INCLUDEPATH "${CMAKE_SOURCE_DIR}/Dependencies/share/OGRE/samples/Common/include/") # This is the path for my precompiled dependencies

	  set(OGRE_BUILD ${CMAKE_SOURCE_DIR}/Dependencies/lib)
	  set(OGRE_SOURCE ${CMAKE_SOURCE_DIR}/Dependencies/ogre)
	  set(OGRE_HOME ${CMAKE_SOURCE_DIR}/Dependencies)
	  set(OGRE_SDK ${CMAKE_SOURCE_DIR}/Dependencies)

	elseif(EXISTS "/usr/local/lib/OGRE/cmake")

	  set(CMAKE_MODULE_PATH "/usr/local/lib/OGRE/cmake/;${CMAKE_MODULE_PATH}")
	  set(OGRE_SAMPLES_INCLUDEPATH "/usr/local/share/OGRE/samples/Common/include/") # We could just *assume* that developers uses this basepath : /usr/local

	elseif(EXISTS "/usr/lib/OGRE/cmake")

	  set(CMAKE_MODULE_PATH "/usr/lib/OGRE/cmake/;${CMAKE_MODULE_PATH}")
	  set(OGRE_SAMPLES_INCLUDEPATH "/usr/share/OGRE/samples/Common/include/") # Otherwise, this one

	else ()
	  message(SEND_ERROR "Failed to find module path.")
	endif(EXISTS "${CMAKE_SOURCE_DIR}/Dependencies/lib/OGRE/cmake")
endif(UNIX)
 
if (CMAKE_BUILD_TYPE STREQUAL "")
  # CMake defaults to leaving CMAKE_BUILD_TYPE empty. This screws up
  # differentiation between debug and release builds.
  set(CMAKE_BUILD_TYPE "RelWithDebInfo" CACHE STRING "Choose the type of build, options are: None (CMAKE_CXX_FLAGS or CMAKE_C_FLAGS used) Debug Release RelWithDebInfo MinSizeRel." FORCE)
endif ()
 
set(CMAKE_DEBUG_POSTFIX "_d")
 
set(CMAKE_INSTALL_PREFIX "${CMAKE_CURRENT_BINARY_DIR}/dist")
 
find_package(OGRE REQUIRED)
 
#if(NOT "${OGRE_VERSION_NAME}" STREQUAL "Cthugha")
#  message(SEND_ERROR "You need Ogre 1.7 Cthugha to build this.")
#endif()
 
find_package(OIS REQUIRED)
 
if(NOT OIS_FOUND)
	message(SEND_ERROR "Failed to find OIS.")
endif()
 
# Find Boost
if (NOT OGRE_BUILD_PLATFORM_IPHONE AND NOT ANDROID)
	if (WIN32 OR APPLE)
		set(Boost_USE_STATIC_LIBS TRUE)
	else ()
		# Statically linking boost to a dynamic Ogre build doesn't work on Linux 64bit
		set(Boost_USE_STATIC_LIBS ${OGRE_STATIC})
	endif ()
	if (MINGW)
		# this is probably a bug in CMake: the boost find module tries to look for
		# boost libraries with name libboost_*, but CMake already prefixes library
		# search names with "lib". This is the workaround.
		set(CMAKE_FIND_LIBRARY_PREFIXES ${CMAKE_FIND_LIBRARY_PREFIXES} "")
	endif ()
	set(Boost_ADDITIONAL_VERSIONS "1.44" "1.44.0" "1.42" "1.42.0" "1.41.0" "1.41" "1.40.0" "1.40" "1.39.0" "1.39" "1.38.0" "1.38" "1.37.0" "1.37" )
	# Components that need linking (NB does not include header-only components like bind)
	set(OGRE_BOOST_COMPONENTS thread date_time)
	find_package(Boost COMPONENTS ${OGRE_BOOST_COMPONENTS} QUIET)
	if (NOT Boost_FOUND)
		# Try again with the other type of libs
		set(Boost_USE_STATIC_LIBS NOT ${Boost_USE_STATIC_LIBS})
		find_package(Boost COMPONENTS ${OGRE_BOOST_COMPONENTS} QUIET)
	endif()
	find_package(Boost QUIET)
 
	# Set up referencing of Boost
	include_directories(${Boost_INCLUDE_DIR})
	add_definitions(-DBOOST_ALL_NO_LIB)
	set(OGRE_LIBRARIES ${OGRE_LIBRARIES} ${Boost_LIBRARIES})
endif()
 
# INSERT HERE ALL SOURCE FILES
set(SRCS
	./src/main.cpp
	./src/ConfigLoader.cpp
	./src/Engine.cpp
	./src/Timer.cpp
	./src/scenes/ApplicationManager.cpp
	./src/scenes/DotScene.cpp
	./src/scenes/Fader.cpp
	./src/components/ComponentManager.cpp
	./src/systems/TransformSystem.cpp
	./src/systems/CameraSystem.cpp
	./src/systems/LightSystem.cpp
	./src/systems/MeshSystem.cpp
	./src/systems/RenderSystem.cpp
	./src/systems/AnimationSystem.cpp
	./src/systems/GUISystem.cpp
	./src/systems/InputSystem.cpp
	./src/systems/PhysicsSystem.cpp
	./src/systems/ScriptingSystem.cpp
	./src/events/EventManager.cpp
	./src/events/EventLogger.cpp
	./src/HashedString.cpp
	./src/debug/DebugView.cpp
	./src/debug/PhysicsDebugDraw.cpp
	./src/debug/DynamicLineDrawer.cpp
)

aux_source_directory(./include/lua/ lua_sources)
set_source_files_properties(${lua_sources} PROPERTIES COMPILE_FLAGS -Wno-deprecated-declarations)

# INSERT HERE ALL HEADER FILES AND INCLUDES 
include_directories( 
	${PROJECT_SOURCE_DIR}/include
	${PROJECT_SOURCE_DIR}/include/view
	${PROJECT_SOURCE_DIR}/include/events
	${PROJECT_SOURCE_DIR}/include/scenes
	${PROJECT_SOURCE_DIR}/include/components
	${PROJECT_SOURCE_DIR}/include/systems
	${PROJECT_SOURCE_DIR}/include/lua
	${PROJECT_SOURCE_DIR}/include/debug
	${OIS_INCLUDE_DIRS}
	${PROJECT_SOURCE_DIR}/Dependencies/bullet/include 
	${PROJECT_SOURCE_DIR}/Dependencies/ogre/RTShaderSystem
	${PROJECT_SOURCE_DIR}/Dependencies/mygui/include 
	${OGRE_INCLUDE_DIRS}
	${OGRE_SAMPLES_INCLUDEPATH}
	${OGRE_Overlay_INCLUDE_DIRS}
)
 
add_executable(${APP_NAME} WIN32 ${SRCS} ${lua_sources})
 
set_target_properties(${APP_NAME} PROPERTIES DEBUG_POSTFIX _d)

# Link executable to libraries - INSERT HERE ALL LIBRARIES 
target_link_libraries(${APP_NAME} ${OGRE_LIBRARIES} ${OGRE_Overlay_LIBRARIES} OgreRTShaderSystem BulletSoftBody BulletDynamics BulletCollision LinearMath ${OIS_LIBRARIES} MyGUIEngine MyGUI.OgrePlatform)
 
file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}) #/${CMAKE_BUILD_TYPE}
file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/assets) #/${CMAKE_BUILD_TYPE}
 
# post-build copy for win32
if(WIN32 AND NOT MINGW)
	#add_custom_command( TARGET ${APP_NAME} PRE_BUILD
	#	COMMAND if not exist .\\${CMAKE_BUILD_TYPE} mkdir .\\${CMAKE_BUILD_TYPE} )
	add_custom_command( TARGET ${APP_NAME} POST_BUILD
		COMMAND copy \"$(TargetPath)\" . ) #\\${CMAKE_BUILD_TYPE}
endif(WIN32 AND NOT MINGW)

if(MINGW OR UNIX)
	set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR})#/${CMAKE_BUILD_TYPE})
endif(MINGW OR UNIX)
 
if(WIN32)
 
	install(TARGETS ${APP_NAME}
		RUNTIME DESTINATION ${PROJECT_BINARY_DIR}#/${CMAKE_BUILD_TYPE}
		CONFIGURATIONS All)
 
	install(DIRECTORY ${CMAKE_SOURCE_DIR}/assets
		DESTINATION ${PROJECT_BINARY_DIR}#/${CMAKE_BUILD_TYPE}
		CONFIGURATIONS Release RelWithDebInfo Debug
	)
 
	install(FILES ${CMAKE_SOURCE_DIR}/config.ini
		DESTINATION ${PROJECT_BINARY_DIR}#/${CMAKE_BUILD_TYPE}
		CONFIGURATIONS Release RelWithDebInfo
	)
 
	install(FILES ${CMAKE_SOURCE_DIR}/config.ini
		DESTINATION ${PROJECT_BINARY_DIR}#/${CMAKE_BUILD_TYPE}
		CONFIGURATIONS Debug
	)
 
        # NOTE: for the 1.7.1 sdk the OIS dll is called OIS.dll instead of libOIS.dll
        # so you'll have to change that to make it work with 1.7.1
	install(FILES ${OGRE_PLUGIN_DIR_REL}/OgreMain.dll
		${OGRE_PLUGIN_DIR_REL}/RenderSystem_Direct3D9.dll
		${OGRE_PLUGIN_DIR_REL}/RenderSystem_GL.dll
		${OGRE_PLUGIN_DIR_REL}/libOIS.dll
		DESTINATION ${PROJECT_BINARY_DIR}#/${CMAKE_BUILD_TYPE}
		CONFIGURATIONS Release RelWithDebInfo
	)
 
	install(FILES ${OGRE_PLUGIN_DIR_DBG}/OgreMain_d.dll
		${OGRE_PLUGIN_DIR_DBG}/RenderSystem_Direct3D9_d.dll
		${OGRE_PLUGIN_DIR_DBG}/RenderSystem_GL_d.dll
		${OGRE_PLUGIN_DIR_DBG}/libOIS_d.dll
		DESTINATION ${PROJECT_BINARY_DIR}#/${CMAKE_BUILD_TYPE}
		CONFIGURATIONS Debug
	)
 
   # as of sdk 1.7.2 we need to copy the boost dll's as well
   # because they're not linked statically (it worked with 1.7.1 though)
   install(FILES ${Boost_DATE_TIME_LIBRARY_RELEASE}
      ${Boost_THREAD_LIBRARY_RELEASE}
      DESTINATION ${PROJECT_BINARY_DIR}#/${CMAKE_BUILD_TYPE}
      CONFIGURATIONS Release RelWithDebInfo
   )
 
   install(FILES ${Boost_DATE_TIME_LIBRARY_DEBUG}
      ${Boost_THREAD_LIBRARY_DEBUG}
      DESTINATION ${PROJECT_BINARY_DIR}#/${CMAKE_BUILD_TYPE}
      CONFIGURATIONS Debug
   )
endif(WIN32)

if(UNIX)
 
	install(TARGETS ${APP_NAME}
		RUNTIME DESTINATION ${PROJECT_BINARY_DIR}#/${CMAKE_BUILD_TYPE}
		CONFIGURATIONS All)
 
	install(DIRECTORY ${CMAKE_SOURCE_DIR}/assets
		DESTINATION ${PROJECT_BINARY_DIR}#/${CMAKE_BUILD_TYPE}
		CONFIGURATIONS Release RelWithDebInfo Debug
	)
 
	install(FILES ${CMAKE_SOURCE_DIR}/config.ini
		DESTINATION ${PROJECT_BINARY_DIR}#/${CMAKE_BUILD_TYPE}
		CONFIGURATIONS Release RelWithDebInfo Debug
	)
 
endif(UNIX)
 
