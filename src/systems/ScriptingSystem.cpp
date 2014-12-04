#include "ScriptingSystem.h"

int lua_log(lua_State *luaState)
{
	try
	{
		int args = lua_gettop(luaState);
		if(args == 1)
		{
			std::string message = lua_tostring(luaState, 1);
			if(!message.empty())
			{
				LOG_INFO(message)
			}
		}
		else if(args == 2)
		{
			std::string message = lua_tostring(luaState, 1);
			std::string level = lua_tostring(luaState, 2);
			if(!message.empty() && !level.empty())
			{
				if (level == "info")
					LOG_INFO(message)
				if (level == "debug")
					LOG_DEBUG(message)
				if (level == "warn")
					LOG_WARN(message)
				if (level == "error")
					LOG_ERROR(message)
			}
		}
		else
		{
			lua_pushstring(luaState, "Too many or too few arguments on function 'log'");
			lua_error(luaState);
		}
	}
	catch(std::exception const &e)
	{
		lua_pushstring(luaState, e.what());
		lua_error(luaState);
	}
}

int lua_queueEvent(lua_State *luaState)
{
	try
	{
		int args = lua_gettop(luaState);
		if(args == 1)
		{
			std::string eventName = lua_tostring(luaState, 1);
			if(!eventName.empty())
			{
				GameEventPtr event(new SystemEvent(eventName));
				safeQueueEvent(event);
			}
		}
		else if(args > 1)
		{
			std::string eventName = lua_tostring(luaState, 1);
			std::string eventType = lua_tostring(luaState, 2);
			if(!eventName.empty() && !eventType.empty())
			{
				if (eventType == "sceneevent")
				{
					std::string sceneName = lua_tostring(luaState, 3);
					GameEventPtr event(new SceneEvent(eventName, sceneName));
					safeQueueEvent(event);
				}
				if (eventType == "entityevent")
				{
					int entityID = lua_tointeger(luaState, 3);
					GameEventPtr event(new EntityEvent(eventName, entityID));
					safeQueueEvent(event);
				}
				if (eventType == "componentevent")
				{
//					int entityID = lua_tointeger(luaState, 3);
//					GameEventPtr event(new ComponentEvent(eventName, entityID));
//					safeQueueEvent(event);
				}
				if (eventType == "animationevent")
				{
					int entityID = lua_tointeger(luaState, 3);
					std::string animationName = lua_tostring(luaState, 4);
					GameEventPtr event(new AnimationEvent(eventName, entityID, animationName));
					safeQueueEvent(event);
				}
				if (eventType == "guievent")
				{
					std::string widgetName = lua_tostring(luaState, 3);
					std::string value = lua_tostring(luaState, 4);
					GameEventPtr event(new GUIEvent(eventName, widgetName, value));
					safeQueueEvent(event);
				}
				if (eventType == "forceevent")
				{
					int entityID = lua_tointeger(luaState, 3);
					float directionX = lua_tonumber(luaState, 4);
					float directionY = lua_tonumber(luaState, 5);
					float directionZ = lua_tonumber(luaState, 6);
					float velocity = lua_tonumber(luaState, 7);
					GameEventPtr event(new ForceEvent(eventName, entityID, directionX, directionY, directionZ, velocity));
					safeQueueEvent(event);
				}
				if (eventType == "floatevent")
				{
					float value = lua_tonumber(luaState, 3);
					GameEventPtr event(new FloatEvent(eventName, value));
					safeQueueEvent(event);
				}
				if (eventType == "colorevent")
				{
					std::string materialName = lua_tostring(luaState, 3);
					float r = lua_tonumber(luaState, 4);
					float g = lua_tonumber(luaState, 5);
					float b = lua_tonumber(luaState, 6);
					float a = lua_tonumber(luaState, 7);
					GameEventPtr event(new ColorEvent(eventName, materialName, r, g, b, a));
					safeQueueEvent(event);
				}
			}
		}
	}
	catch(std::exception const &e)
	{
		lua_pushstring(luaState, e.what());
		lua_error(luaState);
	}
}

int lua_addEventListener(lua_State *luaState)
{
	try
	{
		int args = lua_gettop(luaState);
		if(args == 3)
		{
			std::string eventName = lua_tostring(luaState, 1);
			std::string functionName = lua_tostring(luaState, 2);
			ScriptingSystem* eventListener = (ScriptingSystem*) lua_touserdata(luaState, 3);
			if(!eventListener)
			{
				lua_pushstring(luaState, "Wrong listener object");
				lua_error(luaState);
			}

			if(!eventName.empty() && !functionName.empty())
			{
				safeAddListener(dynamic_cast<IEventListener*>(eventListener), EventType(eventName));
				eventListener->insertEventListener(EventType(eventName), functionName);
			}
		}
		else
		{
			lua_pushstring(luaState, "Too many or too few arguments on function 'addEventListener'");
			lua_error(luaState);
		}
	}
	catch(std::exception const &e)
	{
		lua_pushstring(luaState, e.what());
		lua_error(luaState);
	}
}

int lua_delEventListener(lua_State *luaState)
{
	try
	{
		int args = lua_gettop(luaState);
		if(args == 2)
		{
			std::string eventName = lua_tostring(luaState, 1);
			ScriptingSystem* eventListener = (ScriptingSystem*) lua_touserdata(luaState, 2);
			if(!eventListener)
			{
				lua_pushstring(luaState, "Wrong listener object");
				lua_error(luaState);
			}

			if(!eventName.empty())
			{
				safeDelListener(dynamic_cast<IEventListener*>(eventListener), EventType(eventName));
				eventListener->delEventListener(EventType(eventName));
			}
		}
		else
		{
			lua_pushstring(luaState, "Too many or too few arguments on function 'delEventListener'");
			lua_error(luaState);
		}
	}
	catch(std::exception const &e)
	{
		lua_pushstring(luaState, e.what());
		lua_error(luaState);
	}
}

int lua_createEntity(lua_State *luaState)
{
	try
	{
		int args = lua_gettop(luaState);
		if(args == 1)
		{
			ScriptingSystem* system = (ScriptingSystem*) lua_touserdata(luaState, 1);
			if(!system)
			{
				lua_pushstring(luaState, "Wrong listener object");
				lua_error(luaState);
			}
			else
			{
				int entity = system->getComponentMgr()->createEntity();
				lua_pushinteger(luaState, entity);
				return 1;
			}
		}
		else
		{
			lua_pushstring(luaState, "Too many or too few arguments on function 'createEntity'");
			lua_error(luaState);
		}
	}
	catch(std::exception const &e)
	{
		lua_pushstring(luaState, e.what());
		lua_error(luaState);
	}
}

int lua_destroyEntity(lua_State *luaState)
{
	try
	{
		int args = lua_gettop(luaState);
		if(args == 2)
		{
			ScriptingSystem* system = (ScriptingSystem*) lua_touserdata(luaState, 1);
			int entityID = lua_tointeger(luaState, 2);
			if(!system)
			{
				lua_pushstring(luaState, "Wrong listener object");
				lua_error(luaState);
			}
			else
			{
				system->getComponentMgr()->destroyEntity(entityID);
				return 0;
			}
		}
		else
		{
			lua_pushstring(luaState, "Too many or too few arguments on function 'createEntity'");
			lua_error(luaState);
		}
	}
	catch(std::exception const &e)
	{
		lua_pushstring(luaState, e.what());
		lua_error(luaState);
	}
}

int lua_getEntity(lua_State *luaState)
{
	try
	{
		int args = lua_gettop(luaState);
		if(args == 2)
		{
			ScriptingSystem* system = (ScriptingSystem*) lua_touserdata(luaState, 1);
			std::string entityName = lua_tostring(luaState, 2);
			if(!system || entityName.empty())
			{
				lua_pushstring(luaState, "Wrong listener object");
				lua_error(luaState);
			}
			else
			{
				int entity = system->getComponentMgr()->getEntity(entityName);
				lua_pushinteger(luaState, entity);
				return 1;
			}
		}
		else
		{
			lua_pushstring(luaState, "Too many or too few arguments on function 'createEntity'");
			lua_error(luaState);
		}
	}
	catch(std::exception const &e)
	{
		lua_pushstring(luaState, e.what());
		lua_error(luaState);
	}
}

int lua_addComponent(lua_State *luaState)
{
	try
	{
		int args = lua_gettop(luaState);
		if(args > 2)
		{
			ScriptingSystem* system = (ScriptingSystem*) lua_touserdata(luaState, 1);
			int entity = lua_tointeger(luaState, 2);
			std::string componentType = lua_tostring(luaState, 3);
			if(!system)
			{
				lua_pushstring(luaState, "Wrong listener object");
				lua_error(luaState);
			}
			else
			{
				if (componentType == "transform")
				{
					float x = lua_tonumber(luaState, 4);
					float y = lua_tonumber(luaState, 5);
					float z = lua_tonumber(luaState, 6);
					system->getComponentMgr()->addComponent(entity, *(new TransformComponent(Ogre::Vector3(x,y,z))));
				}
				if (componentType == "mesh")
				{
					std::string entityName = lua_tostring(luaState, 4);
					std::string meshName = lua_tostring(luaState, 5);
					bool castShadows = lua_toboolean(luaState, 6);
					system->getComponentMgr()->addComponent(entity, *(new MeshComponent(entityName, meshName, castShadows)));
				}
				if (componentType == "render")
				{
					std::string nodeName = lua_tostring(luaState, 4);
					system->getComponentMgr()->addComponent(entity, *(new RenderComponent(nodeName)));
				}
				if (componentType == "physics")
				{
					if (args == 8)
					{
						float x = lua_tonumber(luaState, 4);
						float y = lua_tonumber(luaState, 5);
						float z = lua_tonumber(luaState, 6);
						float mass = lua_tonumber(luaState, 7);
						bool isTrigger = lua_toboolean(luaState, 8);
						system->getComponentMgr()->addComponent(entity, *(new PhysicsComponent(btVector3(x,y,z), mass, btVector3(0,0,0), isTrigger)));
					}
					else if (args == 7)
					{
						float radius = lua_tonumber(luaState, 4);
						float height = lua_tonumber(luaState, 5);
						float mass = lua_tonumber(luaState, 6);
						bool isTrigger = lua_toboolean(luaState, 7);
						system->getComponentMgr()->addComponent(entity, *(new PhysicsComponent(radius, height, mass, btVector3(0,0,0), isTrigger)));
					}
					else if (args == 6)
					{
						float radius = lua_tonumber(luaState, 4);
						float mass = lua_tonumber(luaState, 5);
						bool isTrigger = lua_toboolean(luaState, 6);
						system->getComponentMgr()->addComponent(entity, *(new PhysicsComponent(radius, mass, btVector3(0,0,0), isTrigger)));
					}
				}
			}
		}
		else
		{
			lua_pushstring(luaState, "Too many or too few arguments on function 'addComponent'");
			lua_error(luaState);
		}
	}
	catch(std::exception const &e)
	{
		lua_pushstring(luaState, e.what());
		lua_error(luaState);
	}
}

int lua_removeComponent(lua_State *luaState)
{
	try
	{
		int args = lua_gettop(luaState);
		if(args > 2)
		{
			ScriptingSystem* system = (ScriptingSystem*) lua_touserdata(luaState, 1);
			int entity = lua_tointeger(luaState, 2);
			std::string componentType = lua_tostring(luaState, 3);
			if(!system)
			{
				lua_pushstring(luaState, "Wrong listener object");
				lua_error(luaState);
			}
			else
			{
				const std::type_info* type;
				if (componentType == "transform")
				{
					type = &typeid(TransformComponent);
				}
				else if (componentType == "mesh")
				{
					type = &typeid(MeshComponent);
				}
				else if (componentType == "render")
				{
					type = &typeid(RenderComponent);
				}
				else if (componentType == "camera")
				{
					type = &typeid(CameraComponent);
				}
				else if (componentType == "light")
				{
					type = &typeid(LightComponent);
				}
				else if (componentType == "physics")
				{
					type = &typeid(PhysicsComponent);
				}
#if OGRE_PLATFORM == OGRE_PLATFORM_APPLE_IOS
#else
				else if (componentType == "gui")
				{
					type = &typeid(GUIComponent);
				}
#endif
				else if (componentType == "input")
				{
					type = &typeid(InputComponent);
					// TODO: input system isn't an event listener
				}
				GameEventPtr event(new ComponentEvent("RemoveComponent", entity, type));
				safeQueueEvent(event);
			}
		}
		else
		{
			lua_pushstring(luaState, "Too many or too few arguments on function 'addComponent'");
			lua_error(luaState);
		}
	}
	catch(std::exception const &e)
	{
		lua_pushstring(luaState, e.what());
		lua_error(luaState);
	}
}

int lua_setPosition(lua_State *luaState)
{
	try
	{
		int args = lua_gettop(luaState);
		if(args > 2)
		{
			ScriptingSystem* system = (ScriptingSystem*) lua_touserdata(luaState, 1);
			int entity = lua_tointeger(luaState, 2);
			if(!system)
			{
				lua_pushstring(luaState, "Wrong listener object");
				lua_error(luaState);
			}
			else
			{
				float x = lua_tonumber(luaState, 3);
				float y = lua_tonumber(luaState, 4);
				float z = lua_tonumber(luaState, 5);
				dynamic_cast<TransformComponent*>(system->getComponentMgr()->getComponent(entity, &typeid(TransformComponent)).get())->setPosition(Ogre::Vector3(x,y,z));
			}
		}
		else
		{
			lua_pushstring(luaState, "Too many or too few arguments on function 'addComponent'");
			lua_error(luaState);
		}
	}
	catch(std::exception const &e)
	{
		lua_pushstring(luaState, e.what());
		lua_error(luaState);
	}
}

int lua_setRotation(lua_State *luaState)
{
	try
	{
		int args = lua_gettop(luaState);
		if(args > 2)
		{
			ScriptingSystem* system = (ScriptingSystem*) lua_touserdata(luaState, 1);
			int entity = lua_tointeger(luaState, 2);
			if(!system)
			{
				lua_pushstring(luaState, "Wrong listener object");
				lua_error(luaState);
			}
			else
			{
				float w = lua_tonumber(luaState, 3);
				float x = lua_tonumber(luaState, 4);
				float y = lua_tonumber(luaState, 5);
				float z = lua_tonumber(luaState, 6);
				Ogre::Quaternion rotation(Ogre::Radian(Ogre::Degree(w)), Ogre::Vector3(x,y,z));
				Ogre::Quaternion oldRotation = dynamic_cast<TransformComponent*>(system->getComponentMgr()->getComponent(entity, &typeid(TransformComponent)).get())->getRotation();
				dynamic_cast<TransformComponent*>(system->getComponentMgr()->getComponent(entity, &typeid(TransformComponent)).get())->setRotation(rotation*oldRotation);
			}
		}
		else
		{
			lua_pushstring(luaState, "Too many or too few arguments on function 'addComponent'");
			lua_error(luaState);
		}
	}
	catch(std::exception const &e)
	{
		lua_pushstring(luaState, e.what());
		lua_error(luaState);
	}
}

int lua_setScale(lua_State *luaState)
{
	try
	{
		int args = lua_gettop(luaState);
		if(args > 2)
		{
			ScriptingSystem* system = (ScriptingSystem*) lua_touserdata(luaState, 1);
			int entity = lua_tointeger(luaState, 2);
			if(!system)
			{
				lua_pushstring(luaState, "Wrong listener object");
				lua_error(luaState);
			}
			else
			{
				float x = lua_tonumber(luaState, 3);
				float y = lua_tonumber(luaState, 4);
				float z = lua_tonumber(luaState, 5);
				dynamic_cast<TransformComponent*>(system->getComponentMgr()->getComponent(entity, &typeid(TransformComponent)).get())->setScale(Ogre::Vector3(x,y,z));
			}
		}
		else
		{
			lua_pushstring(luaState, "Too many or too few arguments on function 'addComponent'");
			lua_error(luaState);
		}
	}
	catch(std::exception const &e)
	{
		lua_pushstring(luaState, e.what());
		lua_error(luaState);
	}
}

int lua_getPosition(lua_State *luaState)
{
	try
	{
		int args = lua_gettop(luaState);
		if(args == 2)
		{
			ScriptingSystem* system = (ScriptingSystem*) lua_touserdata(luaState, 1);
			int entity = lua_tointeger(luaState, 2);
			if(!system)
			{
				lua_pushstring(luaState, "Wrong listener object");
				lua_error(luaState);
			}
			else
			{
				const Ogre::Vector3 position = dynamic_cast<TransformComponent*>(system->getComponentMgr()->getComponent(entity, &typeid(TransformComponent)).get())->getPosition();
				lua_pushnumber(luaState, position.x);
				lua_pushnumber(luaState, position.y);
				lua_pushnumber(luaState, position.z);
				return 3;
			}
		}
		else
		{
			lua_pushstring(luaState, "Too many or too few arguments on function 'createEntity'");
			lua_error(luaState);
		}
	}
	catch(std::exception const &e)
	{
		lua_pushstring(luaState, e.what());
		lua_error(luaState);
	}
}

int lua_getRotation(lua_State *luaState)
{
	try
	{
		int args = lua_gettop(luaState);
		if(args > 2)
		{
			ScriptingSystem* system = (ScriptingSystem*) lua_touserdata(luaState, 1);
			int entity = lua_tointeger(luaState, 2);
			if(!system)
			{
				lua_pushstring(luaState, "Wrong listener object");
				lua_error(luaState);
			}
			else
			{
				float x = lua_tonumber(luaState, 3);
				float y = lua_tonumber(luaState, 4);
				float z = lua_tonumber(luaState, 5);
				const Ogre::Quaternion rotation = dynamic_cast<TransformComponent*>(system->getComponentMgr()->getComponent(entity, &typeid(TransformComponent)).get())->getRotation();
				Ogre::Degree angle;
				Ogre::Vector3 axis(x,y,z);
				rotation.ToAngleAxis(angle, axis);
				lua_pushnumber(luaState, angle.valueDegrees());
				lua_pushnumber(luaState, axis.x);
				lua_pushnumber(luaState, axis.y);
				lua_pushnumber(luaState, axis.z);
				return 4;
			}
		}
		else
		{
			lua_pushstring(luaState, "Too many or too few arguments on function 'createEntity'");
			lua_error(luaState);
		}
	}
	catch(std::exception const &e)
	{
		lua_pushstring(luaState, e.what());
		lua_error(luaState);
	}
}

int lua_getScale(lua_State *luaState)
{
	try
	{
		int args = lua_gettop(luaState);
		if(args == 2)
		{
			ScriptingSystem* system = (ScriptingSystem*) lua_touserdata(luaState, 1);
			int entity = lua_tointeger(luaState, 2);
			if(!system)
			{
				lua_pushstring(luaState, "Wrong listener object");
				lua_error(luaState);
			}
			else
			{
				const Ogre::Vector3 scale = dynamic_cast<TransformComponent*>(system->getComponentMgr()->getComponent(entity, &typeid(TransformComponent)).get())->getScale();
				lua_pushnumber(luaState, scale.x);
				lua_pushnumber(luaState, scale.y);
				lua_pushnumber(luaState, scale.z);
				return 3;
			}
		}
		else
		{
			lua_pushstring(luaState, "Too many or too few arguments on function 'createEntity'");
			lua_error(luaState);
		}
	}
	catch(std::exception const &e)
	{
		lua_pushstring(luaState, e.what());
		lua_error(luaState);
	}
}

int lua_getScreenWidth(lua_State *luaState)
{
	try
	{
		int args = lua_gettop(luaState);
		if(args == 1)
		{
			ScriptingSystem* system = (ScriptingSystem*) lua_touserdata(luaState, 1);
			if(!system)
			{
				lua_pushstring(luaState, "Wrong listener object");
				lua_error(luaState);
			}
			else
			{
            #if OGRE_PLATFORM == OGRE_PLATFORM_APPLE_IOS
                lua_pushinteger(luaState, system->getRenderWindow()->getWidth()/2);
            #else
				lua_pushinteger(luaState, system->getRenderWindow()->getWidth());
            #endif
				return 1;
			}
		}
		else
		{
			lua_pushstring(luaState, "Too many or too few arguments on function 'createEntity'");
			lua_error(luaState);
		}
	}
	catch(std::exception const &e)
	{
		lua_pushstring(luaState, e.what());
		lua_error(luaState);
	}
}

int lua_getScreenHeight(lua_State *luaState)
{
	try
	{
		int args = lua_gettop(luaState);
		if(args == 1)
		{
			ScriptingSystem* system = (ScriptingSystem*) lua_touserdata(luaState, 1);
			if(!system)
			{
				lua_pushstring(luaState, "Wrong listener object");
				lua_error(luaState);
			}
			else
			{
            #if OGRE_PLATFORM == OGRE_PLATFORM_APPLE_IOS
                lua_pushinteger(luaState, system->getRenderWindow()->getHeight()/2);
            #else
				lua_pushinteger(luaState, system->getRenderWindow()->getHeight());
            #endif
				return 1;
			}
		}
		else
		{
			lua_pushstring(luaState, "Too many or too few arguments on function 'createEntity'");
			lua_error(luaState);
		}
	}
	catch(std::exception const &e)
	{
		lua_pushstring(luaState, e.what());
		lua_error(luaState);
	}
}

void ScriptingSystem::start()
{
	std::string allScripts = "";
	for (std::pair<std::string, std::string> scriptFile : config->getScripts())
	{
		Ogre::DataStreamPtr stream = Ogre::ResourceGroupManager::getSingleton().openResource(scriptFile.second, "Scripts");
		allScripts = allScripts + stream->getAsString();
		systems.push_back(scriptFile.first);
	}

	try
	{
        luaState = luaL_newstate();
        luaL_openlibs(luaState);

        lua_register(luaState, "log", lua_log);
        lua_register(luaState, "queueEvent", lua_queueEvent);
        lua_register(luaState, "addEventListener", lua_addEventListener);
        lua_register(luaState, "delEventListener", lua_delEventListener);
        lua_register(luaState, "createEntity", lua_createEntity);
        lua_register(luaState, "destroyEntity", lua_destroyEntity);
        lua_register(luaState, "getEntity", lua_getEntity);
        lua_register(luaState, "addComponent", lua_addComponent);
        lua_register(luaState, "removeComponent", lua_removeComponent);
        lua_register(luaState, "setPosition", lua_setPosition);
        lua_register(luaState, "getPosition", lua_getPosition);
        lua_register(luaState, "setRotation", lua_setRotation);
        lua_register(luaState, "getRotation", lua_getRotation);
        lua_register(luaState, "setScale", lua_setScale);
        lua_register(luaState, "getScale", lua_getScale);
        lua_register(luaState, "getScreenWidth", lua_getScreenWidth);
        lua_register(luaState, "getScreenHeight", lua_getScreenHeight);

        int iStatus = luaL_loadstring(luaState, strdup(allScripts.c_str()));
        if( iStatus )
        {
            std::cout << "Error: " << lua_tostring(luaState, -1);
            //throw( std::exception( "script error" ) );
        }

        this->loadScripts();

        for (std::string system : systems)
        {
        	lua_getglobal(luaState, system.c_str());
			lua_getfield(luaState, -1, "start");
			lua_remove(luaState, -2);
			lua_pushlightuserdata(luaState, (void*)this);
			lua_pcall(luaState, 1, 0, 0);
        }
    }
    catch( std::exception const &e )
    {
        LOG_ERROR(e.what());
    }

	LOG_DEBUG("Scripting system started");
}

void ScriptingSystem::update(float deltaTime)
{
	for (std::string system : systems)
	{
		lua_getglobal(luaState, system.c_str());
		lua_getfield(luaState, -1, "update");
		lua_pushnumber(luaState, deltaTime);
		lua_remove(luaState, -3);
		lua_pcall(luaState, 1, 0, 0);
	}
}

void ScriptingSystem::stop()
{
	for (std::string system : systems)
	{
		lua_getglobal(luaState, system.c_str());
		lua_getfield(luaState, -1, "stop");
		lua_remove(luaState, -2);
		lua_pcall(luaState, 0, 0, 0);
	}

	for (std::pair<EventType, std::vector<std::string>> listener : eventListeners)
	{
		safeDelListener(dynamic_cast<IEventListener*>(this), listener.first);
	}
	eventListeners.clear();

	if(luaState)
	{
		lua_close(luaState);
	}

	LOG_DEBUG("Scripting system stopped");
}

bool ScriptingSystem::handleEvent(const GameEvent& event)
{
	for (std::string function : eventListeners[event.getEventType()])
	{
		std::istringstream stream(function);
		std::string token;
		int i = 0;

		while(std::getline(stream, token, '.'))
		{
		    if (i == 0)
		    	lua_getglobal(luaState, token.c_str());
		    else
		    	lua_getfield(luaState, i, token.c_str());
		    i--;
		}

		if (event.getEventCategory() == "inputevent")
		{
			const InputEvent& castedEvent = dynamic_cast<const InputEvent&>(event);
			lua_pushinteger(luaState, castedEvent.entityID);
			lua_pushinteger(luaState, castedEvent.x);
			lua_pushinteger(luaState, castedEvent.y);
			lua_pcall(luaState, 3, 0, 0);
		}
		else if (event.getEventCategory() == "sceneevent")
		{
			const SceneEvent& castedEvent = dynamic_cast<const SceneEvent&>(event);
			lua_pushstring(luaState, castedEvent.sceneName.c_str());
			lua_pcall(luaState, 1, 0, 0);
		}
		else if (event.getEventCategory() == "entityevent")
		{
			const EntityEvent& castedEvent = dynamic_cast<const EntityEvent&>(event);
			lua_pushinteger(luaState, castedEvent.entityID);
			lua_pcall(luaState, 1, 0, 0);
		}
		else if (event.getEventCategory() == "componentevent")
		{
			const ComponentEvent& castedEvent = dynamic_cast<const ComponentEvent&>(event);
		}
		else if (event.getEventCategory() == "animationevent")
		{
			const AnimationEvent& castedEvent = dynamic_cast<const AnimationEvent&>(event);
			lua_pushinteger(luaState, castedEvent.entityID);
			lua_pushstring(luaState, castedEvent.animationName.c_str());
			lua_pcall(luaState, 2, 0, 0);
		}
		else if (event.getEventCategory() == "collisionevent")
		{
			const CollisionEvent& castedEvent = dynamic_cast<const CollisionEvent&>(event);
			lua_pushinteger(luaState, castedEvent.entityA);
			lua_pushinteger(luaState, castedEvent.entityB);
			lua_pcall(luaState, 2, 0, 0);
		}
		else
		{
			//const SystemEvent& castedEvent = dynamic_cast<const SystemEvent&>(event);
			lua_pcall(luaState, 0, 0, 0);
		}
	}

	return true;
}

void ScriptingSystem::loadScripts()
{
	int status = lua_pcall(luaState, 0, 0, 0);
	if(status)
	{
		std::cout << "Error: " << lua_tostring(luaState, -1);
		//throw( std::exception( "script error" ) );
	}
}

void ScriptingSystem::insertEventListener(EventType eventType, std::string functionName)
{
	eventListeners[eventType].push_back(functionName);
}

void ScriptingSystem::delEventListener(EventType eventType)
{
	eventListeners.erase(eventType);
}
