# This file builds a Xcode project for iOS

cmake_minimum_required(VERSION 2.6)

# INSERT NAME FOR BINARY HERE
set(APP_NAME engine)
 
set(CMAKE_C_COMPILER "clang")
set(CMAKE_CXX_COMPILER "clang++")

project(${APP_NAME}_project)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++11")

# QuartzCore is necessary
set(CMAKE_EXE_LINKER_FLAGS
    "-framework AudioToolbox -framework CoreGraphics -framework QuartzCore -framework UIKit"
)

cmake_policy(SET CMP0021 OLD)

set(OGRE_BUILD_PLATFORM_APPLE_IOS TRUE)

set(OGRE_SOURCE ${PROJECT_SOURCE_DIR}/../../ogre/source)
set(OGRE_BUILD ${PROJECT_SOURCE_DIR}/../../ogre/build)
set(OGRE_HOME ${PROJECT_SOURCE_DIR}/../../ogre/source)
set(OGRE_SDK ${PROJECT_SOURCE_DIR}/../../ogre/source)

set( CMAKE_MODULE_PATH
   ${CMAKE_MODULE_PATH}
   ${OGRE_SOURCE}/CMake/Packages
   ${OGRE_SOURCE}/CMake/Utils
   ${PROJECT_SOURCE_DIR}
   )

set (OGRE_CONFIG_INCLUDE_DIR ${OGRE_BUILD}/include)
set (OGRE_INCLUDE_DIR ${OGRE_SOURCE}/OgreMain/include)
set (OGRE_LIBRARY_DBG ${OGRE_BUILD}/lib/iphoneos/Debug)
set (DEPENDENCIES 
	${OGRE_BUILD}/lib/iphoneos/Debug/libOgreMainStatic.a
	${OGRE_BUILD}/lib/iphoneos/Debug/libOgreOverlayStatic.a
	${OGRE_BUILD}/lib/iphoneos/Debug/libOgrePagingStatic.a
	${OGRE_BUILD}/lib/iphoneos/Debug/libOgrePropertyStatic.a
	${OGRE_BUILD}/lib/iphoneos/Debug/libOgreRTShaderSystemStatic.a
	${OGRE_BUILD}/lib/iphoneos/Debug/libOgreTerrainStatic.a
	${OGRE_BUILD}/lib/iphoneos/Debug/libOgreVolumeStatic.a
	${OGRE_BUILD}/lib/iphoneos/Debug/libPlugin_OctreeSceneManagerStatic.a
	${OGRE_BUILD}/lib/iphoneos/Debug/libPlugin_ParticleFXStatic.a
	${OGRE_BUILD}/lib/iphoneos/Debug/libRenderSystem_GLES2Static.a
	${OGRE_BUILD}/lib/iphoneos/Debug/libRenderSystem_GLESStatic.a
	${OGRE_SOURCE}/iOSDependencies/lib/Debug/libFreeImage.a
	${OGRE_SOURCE}/iOSDependencies/lib/Debug/libFreeType.a
	${OGRE_SOURCE}/iOSDependencies/lib/Debug/libglsl_optimizer.a
	${OGRE_SOURCE}/iOSDependencies/lib/Debug/libhlsl2glsl.a
	${OGRE_SOURCE}/iOSDependencies/lib/Debug/libOIS.a
	${OGRE_SOURCE}/iOSDependencies/lib/Debug/libzzip.a
	${OGRE_SOURCE}/iOSDependencies/lib/libboost_atomic.a
	${OGRE_SOURCE}/iOSDependencies/lib/libboost_chrono.a
	${OGRE_SOURCE}/iOSDependencies/lib/libboost_date_time.a
	${OGRE_SOURCE}/iOSDependencies/lib/libboost_system.a
	${OGRE_SOURCE}/iOSDependencies/lib/libboost_thread.a
	${PROJECT_SOURCE_DIR}/../iOSDependencies/bullet/lib/libBulletSoftBody_xcode4_debug.a
	${PROJECT_SOURCE_DIR}/../iOSDependencies/bullet/lib/libBulletDynamics_xcode4_debug.a
	${PROJECT_SOURCE_DIR}/../iOSDependencies/bullet/lib/libBulletCollision_xcode4_debug.a
	${PROJECT_SOURCE_DIR}/../iOSDependencies/bullet/lib/libLinearMath_xcode4_debug.a
	
	#${PROJECT_SOURCE_DIR}/../iOSDependencies/ogre/lib/Release/libFreeImage.a
	#${PROJECT_SOURCE_DIR}/../iOSDependencies/ogre/lib/Release/libfreetype.a
	#${PROJECT_SOURCE_DIR}/../iOSDependencies/ogre/lib/Release/libhlsl2glsl.a
	#${PROJECT_SOURCE_DIR}/../iOSDependencies/ogre/lib/Release/libmesaglsl2.a
	#${PROJECT_SOURCE_DIR}/../iOSDependencies/ogre/lib/Release/libOgreMainStatic.a
	#${PROJECT_SOURCE_DIR}/../iOSDependencies/ogre/lib/Release/libOgreRTShaderSystemStatic.a
	#${PROJECT_SOURCE_DIR}/../iOSDependencies/ogre/lib/Release/libOIS.a
	#${PROJECT_SOURCE_DIR}/../iOSDependencies/ogre/lib/Release/libPlugin_OctreeSceneManagerStatic.a
	#${PROJECT_SOURCE_DIR}/../iOSDependencies/ogre/lib/Release/libRenderSystem_GLES2Static.a
	#${PROJECT_SOURCE_DIR}/../iOSDependencies/ogre/lib/Release/libRenderSystem_GLESStatic.a
	#${PROJECT_SOURCE_DIR}/../iOSDependencies/ogre/lib/Release/libzlib.a
	#${PROJECT_SOURCE_DIR}/../iOSDependencies/ogre/lib/Release/libzziplib.a
	
	#${PROJECT_SOURCE_DIR}/../iOSDependencies/bullet/lib/libbullet.a
	${PROJECT_SOURCE_DIR}/../iOSDependencies/lua/liblua.a
	
	#${PROJECT_SOURCE_DIR}/../iOSDependencies/mygui/lib/Release/libMyGUIEngineStatic.a
	#${PROJECT_SOURCE_DIR}/../iOSDependencies/mygui/lib/Release/libMyGUI.OgrePlatform.a
)

set (FreeImage_INCLUDE_DIR ${OGRE_SOURCE}/iOSDependencies/include)
set (FreeImage_LIBRARY_DBG ${OGRE_SOURCE}/iOSDependencies/lib/Debug)

set (FREETYPE_INCLUDE_DIR ${OGRE_SOURCE}/iOSDependencies/include/freetype)
set (FREETYPE_LIBRARY_DBG ${OGRE_SOURCE}/iOSDependencies/lib/Debug)

set (OpenGL_INCLUDE_DIR ${OGRE_SOURCE}/iOSDependencies/include)
set (OpenGL_LIBRARY_DBG ${OGRE_SOURCE}/iOSDependencies/lib/Debug)
set (OpenGLES_INCLUDE_DIR ${OGRE_SOURCE}/iOSDependencies/include)
set (OpenGLES_LIBRARY_DBG ${OGRE_SOURCE}/iOSDependencies/lib/Debug)
set (OpenGLES2_INCLUDE_DIR ${OGRE_SOURCE}/iOSDependencies/include)
set (OpenGLES2_LIBRARY_DBG ${OGRE_SOURCE}/iOSDependencies/lib/Debug)

set (ZZip_INCLUDE_DIR ${OGRE_SOURCE}/iOSDependencies/include/zzip)
set (ZZip_LIBRARY_DBG ${OGRE_SOURCE}/iOSDependencies/lib/Debug)

set (OIS_INCLUDE_DIR ${OGRE_SOURCE}/iOSDependencies/include/OIS)
set (OIS_LIBRARY_DBG ${OGRE_SOURCE}/iOSDependencies/lib/Debug)

# Compile Ogre fors iOS: iOSDependencies in source, make build dir, run cmake .. -D OGRE_BUILD_PLATFORM_APPLE_IOS=1 -G Xcode, open Xcode Project and set optimization level to "fastest, smallest", build samplebrowser or other targets, install pkg-config and have the FindPkgCofnig.cmake in CMAKE_MODULE_PATH
# Compile project with ogre static ios libraries: go to source, make build dir, run cmake .. -D CMAKE_TOOLCHAIN_FILE=iOS.cmake -G Xcode twice, mark main.cpp in xcode as Objective-C++ Source, LLVM compiler, "engine" in info.plist as BundleExecutable, build active YES, build phases->link binary with libraries->opengles and the three libstdc++, to build settings:
# Other linker flags: -lOgreMainStatic -lOgreOverlayStatic -lOgreRTShaderSystemStatic -lPlugin_OctreeSceneManagerStatic -lPlugin_ParticleFXStatic -lRenderSystem_GLESStatic -lRenderSystem_GLES2Static -lboost_atomic -lboost_chrono -lboost_date_time -lboost_system -lboost_thread -lFreeImage -lFreeType -lglsl_optimizer -lhlsl2glsl -lOIS -lzzip, Library Search Paths: /Users/1langbeh/Downloads/ogre/build-1.9-ios/lib/iphoneos/Debug /Users/1langbeh/Downloads/ogre/source-1.9-new/iOSDependencies/lib /Users/1langbeh/Downloads/ogre/source-1.9-new/iOSDependencies/lib/Debug
# TODO: in findogre.cmake set (OGRE_LIBRARY "/Users/1langbeh/Downloads/ogre/build-1.9-ios/lib/iphoneos/Debug") should be found by find_library. doesnt work with static libraries
# In Info.plist the Executable file (CFBundleExecutable) must be ${EXECUTABLE_NAME}"

if (CMAKE_BUILD_TYPE STREQUAL "")
  # CMake defaults to leaving CMAKE_BUILD_TYPE empty. This screws up
  # differentiation between debug and release builds.
  set(CMAKE_BUILD_TYPE "Debug" CACHE STRING "Choose the type of build, options are: None (CMAKE_CXX_FLAGS or CMAKE_C_FLAGS used) Debug Release RelWithDebInfo MinSizeRel." FORCE)
endif ()

set(OGRE_STATIC TRUE)

find_package(OGRE REQUIRED)

find_package(OIS REQUIRED)

if(NOT OIS_FOUND)
	message(SEND_ERROR "Failed to find OIS.")
endif()

# Configure Browser demo build

include_directories( 
	${PROJECT_SOURCE_DIR}/../include
	${PROJECT_SOURCE_DIR}/../include/view
	${PROJECT_SOURCE_DIR}/../include/events
	${PROJECT_SOURCE_DIR}/../include/scenes
	${PROJECT_SOURCE_DIR}/../include/components
	${PROJECT_SOURCE_DIR}/../include/systems
	${PROJECT_SOURCE_DIR}/../include/lua
	${PROJECT_SOURCE_DIR}/../include/debug
	${OIS_INCLUDE_DIRS}
	${PROJECT_SOURCE_DIR}/../iOSDependencies/bullet/include #/BulletCollision
#	${PROJECT_SOURCE_DIR}/../iOSDependencies/include/bullet/BulletDynamics
#	${PROJECT_SOURCE_DIR}/../iOSDependencies/include/bullet/BulletSoftBody
#	${PROJECT_SOURCE_DIR}/../iOSDependencies/include/bullet/LinearMath
	#${PROJECT_SOURCE_DIR}/../iOSDependencies/mygui/include
	${PROJECT_SOURCE_DIR}/../iOSDependencies/ogre/include
	${PROJECT_SOURCE_DIR}/../iOSDependencies/ogre/include/boost
	${PROJECT_SOURCE_DIR}/../iOSDependencies/ogre/include/freetype
	${PROJECT_SOURCE_DIR}/../iOSDependencies/ogre/include/freetype/config
	${PROJECT_SOURCE_DIR}/../iOSDependencies/ogre/include/freetype/internal
	${PROJECT_SOURCE_DIR}/../iOSDependencies/ogre/include/OIS
	${PROJECT_SOURCE_DIR}/../iOSDependencies/ogre/include/OIS/iphone
	${PROJECT_SOURCE_DIR}/../iOSDependencies/ogre/include/zzip
	${PROJECT_SOURCE_DIR}/../iOSDependencies/ogre/include/OGRE
	${PROJECT_SOURCE_DIR}/../iOSDependencies/ogre/include/OGRE/iOS
	${PROJECT_SOURCE_DIR}/../iOSDependencies/ogre/include/OGRE/Overlay
	${PROJECT_SOURCE_DIR}/../iOSDependencies/ogre/include/OGRE/Paging
	${PROJECT_SOURCE_DIR}/../iOSDependencies/ogre/include/OGRE/Plugins
	${PROJECT_SOURCE_DIR}/../iOSDependencies/ogre/include/OGRE/Plugins/OctreeSceneManager
	${PROJECT_SOURCE_DIR}/../iOSDependencies/ogre/include/OGRE/Plugins/OctreeZone
	${PROJECT_SOURCE_DIR}/../iOSDependencies/ogre/include/OGRE/Plugins/BSPSceneManager
	${PROJECT_SOURCE_DIR}/../iOSDependencies/ogre/include/OGRE/Plugins/ParticleFX
	${PROJECT_SOURCE_DIR}/../iOSDependencies/ogre/include/OGRE/Plugins/PCZSceneManager
	${PROJECT_SOURCE_DIR}/../iOSDependencies/ogre/include/OGRE/Property
	${PROJECT_SOURCE_DIR}/../iOSDependencies/ogre/include/OGRE/RTShaderSystem
	${PROJECT_SOURCE_DIR}/../iOSDependencies/ogre/include/OGRE/Terrain
	${PROJECT_SOURCE_DIR}/../iOSDependencies/ogre/include/OGRE/Threading
	${PROJECT_SOURCE_DIR}/../iOSDependencies/ogre/include/OGRE/Volume
	${PROJECT_SOURCE_DIR}/../iOSDependencies/ogre/include/OGRE/RenderSystems/GLES
	${PROJECT_SOURCE_DIR}/../iOSDependencies/ogre/include/OGRE/RenderSystems/GLES/APPLE
	${PROJECT_SOURCE_DIR}/../iOSDependencies/ogre/include/OGRE/RenderSystems/GLES2
	${PROJECT_SOURCE_DIR}/../iOSDependencies/ogre/include/OGRE/RenderSystems/GLES2/APPLE
	${PROJECT_SOURCE_DIR}/../iOSDependencies/ogre/include/OGRE/RenderSystems/GLES2/GLES2
	${PROJECT_SOURCE_DIR}/../iOSDependencies/ogre/include/OGRE/RenderSystems/GLES2/GLSLES
	${PROJECT_SOURCE_DIR}/../iOSDependencies/ogre/include/OGRE/RenderSystems/GLES2/KHR
	${PROJECT_SOURCE_DIR}/../iOSDependencies/ogre/include/OGRE/RenderSystems/GLES2/StateCacheManager
	#${OGRE_SOURCE}/Components/RTShaderSystem/include
)

set(SOURCE_FILES
	../src/main.cpp
	../src/ConfigLoader.cpp
	../src/Engine.cpp
	../src/Timer.cpp
	../src/scenes/ApplicationManager.cpp
	../src/scenes/DotScene.cpp
	../src/scenes/Fader.cpp
	../src/components/ComponentManager.cpp
	../src/systems/TransformSystem.cpp
	../src/systems/CameraSystem.cpp
	../src/systems/LightSystem.cpp
	../src/systems/MeshSystem.cpp
	../src/systems/RenderSystem.cpp
	../src/systems/AnimationSystem.cpp
	#../src/systems/GUISystem.cpp
	../src/systems/iOSInputSystem.cpp
	../src/systems/InputGestureView.mm
	../src/systems/PhysicsSystem.cpp
	../src/systems/ScriptingSystem.cpp
	../src/events/EventManager.cpp
	../src/events/EventLogger.cpp
	../src/HashedString.cpp
	#../src/debug/DebugView.cpp
	../src/debug/PhysicsDebugDraw.cpp
	../src/debug/DynamicLineDrawer.cpp
)

# Is necessary for xcode to find info.plist
set(RESOURCES
	Info.plist
)

#if (OGRE_BUILD_PLATFORM_APPLE_IOS)
  set_source_files_properties(${CMAKE_CURRENT_SOURCE_DIR}/../src/main.cpp PROPERTIES COMPILE_FLAGS "-x objective-c++")
#endif ()

# Is necessary for seeing the info.plist in the resources directory in xcode
set_source_files_properties(${RESOURCES} PROPERTIES MACOSX_PACKAGE_LOCATION Resources)

add_executable(${APP_NAME} MACOSX_BUNDLE ${SOURCE_FILES} ${RESOURCES})

target_link_libraries(${APP_NAME} ${DEPENDENCIES}) #OgreRTShaderSystem

SET_TARGET_PROPERTIES(${APP_NAME} PROPERTIES MACOSX_BUNDLE_INFO_PLIST ${CMAKE_CURRENT_SOURCE_DIR}/Info.plist)

if (OGRE_BUILD_PLATFORM_APPLE_IOS)
  set_target_properties(${APP_NAME} PROPERTIES XCODE_ATTRIBUTE_TARGETED_DEVICE_FAMILY "1,2")
  set(OGRE_SAMPLE_CONTENTS_PATH build/Debug-iphoneos/${APP_NAME}.app)
      add_custom_command(TARGET ${APP_NAME} POST_BUILD
        #COMMAND mkdir ARGS -p ${OGRE_BINARY_DIR}/lib/$(CONFIGURATION)/
        COMMAND ditto "${PROJECT_SOURCE_DIR}/../assets" "${OGRE_SAMPLE_CONTENTS_PATH}/Media"
      )
endif()

# Just for testing: signing
#SET_TARGET_PROPERTIES(${APP_NAME} PROPERTIES XCODE_ATTRIBUTE_CODE_SIGN_IDENTITY "xxx")
