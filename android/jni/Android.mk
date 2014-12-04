LOCAL_PATH := $(call my-dir)


# lua
#

include $(CLEAR_VARS)
LOCAL_MODULE    := lua
LOCAL_SRC_FILES := ../../include/lua/lapi.c      ../../include/lua/lcorolib.c  ../../include/lua/ldump.c   ../../include/lua/llex.c      ../../include/lua/lopcodes.c  ../../include/lua/lstrlib.c  ../../include/lua/lvm.c ../../include/lua/lauxlib.c   ../../include/lua/lctype.c    ../../include/lua/lfunc.c   ../../include/lua/lmathlib.c  ../../include/lua/loslib.c    ../../include/lua/ltable.c   ../../include/lua/lzio.c ../../include/lua/lbaselib.c  ../../include/lua/ldblib.c    ../../include/lua/lgc.c     ../../include/lua/lmem.c      ../../include/lua/lparser.c   ../../include/lua/ltablib.c ../../include/lua/lbitlib.c   ../../include/lua/ldebug.c    ../../include/lua/linit.c   ../../include/lua/loadlib.c   ../../include/lua/lstate.c    ../../include/lua/ltm.c ../../include/lua/lcode.c     ../../include/lua/ldo.c       ../../include/lua/liolib.c  ../../include/lua/lobject.c   ../../include/lua/lstring.c   ../../include/lua/lundump.c


include $(BUILD_STATIC_LIBRARY)


#engine
#

   
    include $(CLEAR_VARS)
    LOCAL_MODULE    := AffentanzEngine

	LOCAL_C_INCLUDES := $(LOCAL_PATH)/../../include/lua

    LOCAL_LDLIBS	:= -landroid -lc -lm -ldl -llog -lEGL -lGLESv2
    LOCAL_LDLIBS	+= -L$(LOCAL_PATH)/../../../Dependencies/ogre-android/lib -L$(LOCAL_PATH)/../../../Dependencies/ogre/AndroidDependencies/lib/armeabi-v7a -L$(LOCAL_PATH)/../../../Dependencies/bullet-android -L$(LOCAL_PATH)/../../../Dependencies/mygui/android-build/lib

    LOCAL_LDLIBS	+= -lPlugin_ParticleFXStatic -lPlugin_OctreeSceneManagerStatic -lRenderSystem_GLES2Static -lOgreRTShaderSystemStatic -lOgreOverlayStatic -lOgreTerrainStatic -lOgrePagingStatic -lOgreVolumeStatic -lOgreMainStatic
    LOCAL_LDLIBS	+= -lzzip -lz -lFreeImage -lMyGUI.OgrePlatform -lMyGUIEngineStatic -lfreetype -lOIS  $(LOCAL_PATH)/../../../Dependencies/ogre-android/systemlibs/armeabi-v7a/libsupc++.a $(LOCAL_PATH)/../../../Dependencies/ogre-android/systemlibs/armeabi-v7a/libstdc++.a -lBulletSoftBody -lBulletDynamics -lBulletCollision -lLinearMath $(LOCAL_PATH)/../obj/local/armeabi-v7a/libcpufeatures.a
    LOCAL_STATIC_LIBRARIES := android_native_app_glue cpufeatures lua
    
    LOCAL_CFLAGS := -DGL_GLEXT_PROTOTYPES=1 -I$(LOCAL_PATH)/../../../Dependencies/bullet-2.82-r2704/src -I$(LOCAL_PATH)/../../../Dependencies/ogre/include -I$(LOCAL_PATH)/../../../Dependencies/ogre/OgreMain/include -I$(LOCAL_PATH)/../../../Dependencies/ogre/RenderSystems/GLES2/include -I$(LOCAL_PATH)/../../../Dependencies/ogre/RenderSystems/GLES2/include/EGL
    LOCAL_CFLAGS += -I$(LOCAL_PATH)/../../../Dependencies/ogre/Components/RTShaderSystem/include -I$(LOCAL_PATH)/../../../Dependencies/ogre/Components/Overlay/include -I$(LOCAL_PATH)/../../../Dependencies/ogre/Components/Volume/include -I$(LOCAL_PATH)/../../../Dependencies/ogre/Components/Terrain/include -I$(LOCAL_PATH)/../../../Dependencies/ogre/Components/Paging/include -I$(LOCAL_PATH)/../../../Dependencies/mygui/MyGUIEngine/include -I$(LOCAL_PATH)/../../../Dependencies/mygui/Platforms/Ogre/OgrePlatform/include 
    LOCAL_CFLAGS += -I$(LOCAL_PATH)/../../../Dependencies/ogre/PlugIns/ParticleFX/include -I$(LOCAL_PATH)/../../../Dependencies/ogre/PlugIns/OctreeSceneManager/include 
    LOCAL_CFLAGS += -I$(LOCAL_PATH)/../../../Dependencies/ogre-android/include -I$(LOCAL_PATH)/../../../Dependencies/ogre/AndroidDependencies/include -I$(LOCAL_PATH)/../../../Dependencies/ogre/AndroidDependencies/include/OIS -I$(LOCAL_PATH)/../../../Dependencies/ogre-android/Samples/include -I$(LOCAL_PATH)/../../include -I$(LOCAL_PATH)/../../include/events -I$(LOCAL_PATH)/../../include/scenes -I$(LOCAL_PATH)/../../include/components -I$(LOCAL_PATH)/../../include/systems -I$(LOCAL_PATH)/../../include/debug
    LOCAL_CFLAGS += -fexceptions -frtti -x c++ -D___ANDROID___ -DANDROID -DZZIP_OMIT_CONFIG_H -DINCLUDE_RTSHADER_SYSTEM=1 
    
	LOCAL_CFLAGS += -std=c++11
	
	LOCAL_SRC_FILES := ../../src/main.cpp ../../src/Engine.cpp ../../src/ConfigLoader.cpp ../../src/scenes/ApplicationManager.cpp ../../src/scenes/DotScene.cpp ../../src/scenes/Fader.cpp ../../src/components/ComponentManager.cpp ../../src/Timer.cpp ../../src/events/EventManager.cpp ../../src/events/EventLogger.cpp ../../src/HashedString.cpp ../../src/systems/TransformSystem.cpp ../../src/systems/CameraSystem.cpp ../../src/systems/LightSystem.cpp ../../src/systems/MeshSystem.cpp ../../src/systems/RenderSystem.cpp ../../src/systems/AnimationSystem.cpp ../../src/systems/GUISystem.cpp ../../src/systems/PhysicsSystem.cpp ../../src/systems/ScriptingSystem.cpp ../../src/systems/TouchInputSystem.cpp ../../src/debug/DebugView.cpp ../../src/debug/DynamicLineDrawer.cpp ../../src/debug/PhysicsDebugDraw.cpp 
	
    include $(BUILD_SHARED_LIBRARY) 
      
    $(call import-module,android/cpufeatures) 
    $(call import-module,android/native_app_glue) 
