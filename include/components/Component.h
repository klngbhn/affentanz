#pragma once

#include "Ogre.h"
#if OGRE_PLATFORM == OGRE_PLATFORM_APPLE_IOS
#else
#include "MyGUI.h"
#endif
#include "OISEvents.h"
#include "OISInputManager.h"
#include "OISMouse.h"
#include "OISKeyboard.h"
#include "OISJoyStick.h"
#include "OISMultiTouch.h"
#include "btBulletDynamicsCommon.h"

#include "GameEvent.h"
#include "IEventManager.h"
#include "Log.h"

class Component
{
public:
	Component() {}
	virtual ~Component() {}
	int entityID;
};

struct TransformComponent: public Component
{
public:
	TransformComponent()
	{
		this->position = Ogre::Vector3(0,0,0);
		this->rotation = Ogre::Quaternion(1,0,0,0);
		this->scale = Ogre::Vector3(1,1,1);
	}
	TransformComponent(const Ogre::Vector3& position, const Ogre::Quaternion& rotation, const Ogre::Vector3& scale)
	{
		this->position = position;
		this->rotation = rotation;
		this->scale = scale;
	}
	TransformComponent(const Ogre::Vector3& position)
	{
		this->position = position;
		this->rotation = Ogre::Quaternion(1,0,0,0);
		this->scale = Ogre::Vector3(1,1,1);
	}
	virtual ~TransformComponent() {}

	const Ogre::Vector3& getPosition() const {
		return position;
	}

	void setPosition(const Ogre::Vector3& position) {
		this->position = position;
	}

	const Ogre::Quaternion& getRotation() const {
		return rotation;
	}

	void setRotation(const Ogre::Quaternion& rotation) {
		this->rotation = rotation;
	}

	const Ogre::Vector3& getScale() const {
		return scale;
	}

	void setScale(const Ogre::Vector3& scale) {
		this->scale = scale;
	}

private:
	Ogre::Vector3 position;
	Ogre::Quaternion rotation;
	Ogre::Vector3 scale;
};


struct MeshComponent: public Component
{
public:
	MeshComponent(std::string entityName, std::string meshName, bool castShadows)
	{
		this->entityName = entityName;
		this->meshName = meshName;
		this->castShadows = castShadows;
		this->isPlane = false;
		this->entity = 0;
	}
	MeshComponent(std::string entityName, std::string meshName, std::string material, int width, int height, int xSegs, int ySegs, Ogre::Vector3 normal, Ogre::Vector3 upVector, bool castShadows, bool hasNormals, int distance, int numTexCoordSets, float uTile, float vTile)
	{
		this->entityName = entityName;
		this->meshName = meshName;
		this->width = width;
		this->height = height;
		this->xSegs = xSegs;
		this->ySegs = ySegs;
		this->normal = normal;
		this->upVector = upVector;
		this->castShadows = castShadows;
		this->isPlane = true;
		this->hasNormals = hasNormals;
		this->material = material;
		this->distance = distance;
		this->numTexCoordSets = numTexCoordSets;
		this->uTile = uTile;
		this->vTile = vTile;
		this->entity = 0;
	}
	virtual ~MeshComponent() {}

	Ogre::Entity* entity;
	std::string entityName;
	std::string meshName;
	bool castShadows;
	bool isPlane;

	int width;
	int height;
	int xSegs;
	int ySegs;
	Ogre::Vector3 normal;
	Ogre::Vector3 upVector;
	bool hasNormals;
	std::string material;
	int distance;
	int numTexCoordSets;
	float uTile;
	float vTile;
};

struct RenderComponent: public Component
{
public:
	RenderComponent(std::string nodeName)
	{
		this->nodeName = nodeName;
		this->node = 0;
	}
	virtual ~RenderComponent() {}

	Ogre::SceneNode* node;
	std::string nodeName;
};

struct AnimationComponent: public Component
{
public:
	AnimationComponent(std::string animationName) { this->animationName = animationName; this->animation = 0; }
	virtual ~AnimationComponent() {}

	Ogre::AnimationState* animation;
	std::string animationName;
};

struct CameraComponent: public Component
{
public:
	CameraComponent(std::string cameraName, Ogre::ColourValue backgroundColor, float nearClipDistance, float farClipDistance, Ogre::Vector3 lookAt)
	{
		this->cameraName = cameraName;
		this->backgroundColor = backgroundColor;
		this->nearClipDistance = nearClipDistance;
		this->farClipDistance = farClipDistance;
		this->lookAt = lookAt;
		this->camera = 0;
		this->viewport = 0;
	}
	virtual ~CameraComponent() {}

	Ogre::Camera* camera;
	Ogre::Viewport* viewport;
	std::string cameraName;
	Ogre::ColourValue backgroundColor;
	float nearClipDistance;
	float farClipDistance;
	Ogre::Vector3 lookAt;
};

struct LightComponent: public Component
{
public:
	LightComponent(std::string lightName, Ogre::Light::LightTypes lightType, Ogre::ColourValue diffuseColor, Ogre::ColourValue specularColor, Ogre::Vector3 direction)
	{
		this->lightName = lightName;
		this->lightType = lightType;
		this->diffuseColor = diffuseColor;
		this->specularColor = specularColor;
		this->direction = direction;
		this->light = 0;
	}
	virtual ~LightComponent() {}

	Ogre::Light* light;
	std::string lightName;
	Ogre::Light::LightTypes lightType;
	Ogre::ColourValue diffuseColor;
	Ogre::ColourValue specularColor;
	Ogre::Vector3 direction;
};

enum CollisionShape
{
	Box,
	Sphere,
	Capsule,
	ConvexMesh
};

struct PhysicsComponent: public Component
{
public:
	PhysicsComponent(btVector3 dimensions, btScalar mass, btVector3 localInertia, bool isTrigger)
	{
		shape = CollisionShape::Box;
		this->dimensions = dimensions;
		this->mass = mass;
		this->localInertia = localInertia;
		this->isTrigger = isTrigger;
		this->body = 0;
	}
	PhysicsComponent(btScalar radius, btScalar mass, btVector3 localInertia, bool isTrigger)
	{
		shape = CollisionShape::Sphere;
		this->radius = radius;
		this->mass = mass;
		this->localInertia = localInertia;
		this->isTrigger = isTrigger;
		this->body = 0;
	}
	PhysicsComponent(btScalar radius, btScalar height, btScalar mass, btVector3 localInertia, bool isTrigger)
	{
		shape = CollisionShape::Capsule;
		this->radius = radius;
		this->height = height;
		this->mass = mass;
		this->localInertia = localInertia;
		this->isTrigger = isTrigger;
		this->body = 0;
	}

	virtual ~PhysicsComponent() {}

	btRigidBody* body;

	CollisionShape shape;
	btVector3 dimensions;
	btScalar radius;
	btScalar height;

	btScalar mass;
	btVector3 localInertia;
	bool isTrigger;
};

#if OGRE_PLATFORM == OGRE_PLATFORM_APPLE_IOS
#else
struct GUIComponent: public Component
{
public:
	GUIComponent(std::string layoutName, std::map<std::string, std::string> buttonPressedMap)
	{
		this->layoutName = layoutName;
		this->buttonPressedMap = buttonPressedMap;
	}
	virtual ~GUIComponent() {}

	MyGUI::VectorWidgetPtr guiLayout;
	std::string layoutName;
	std::map<std::string, std::string> buttonPressedMap;

	void initDelegates()
	{
		for (std::pair<std::string, std::string> input : buttonPressedMap)
		{
			MyGUI::ButtonPtr button = MyGUI::Gui::getInstancePtr()->findWidget<MyGUI::Button>(input.first);
			button->eventMouseButtonClick += MyGUI::newDelegate(this, &GUIComponent::buttonPressed);
		}
	}

	void buttonPressed(MyGUI::WidgetPtr sender)
	{
		for (std::pair<std::string, std::string> input : buttonPressedMap)
		{
			if (sender->getName() == input.first)
			{
				GameEventPtr event(new InputEvent(input.second, this->entityID));
				safeQueueEvent(event);
			}
		}
	}
};
#endif

struct InputComponent: public Component
{
public:
	InputComponent(std::map<OIS::KeyCode, std::string> keyPressedMap, std::map<OIS::KeyCode, std::string> keyReleasedMap, std::map<OIS::MouseButtonID, std::string> mousePressedMap, std::map<OIS::MouseButtonID, std::string> mouseReleasedMap, std::string mouseMoved, std::string touchPressed, std::string touchReleased, std::string touchMoved)
	{
		this->keyPressedMap = keyPressedMap;
		this->keyReleasedMap = keyReleasedMap;
		this->mousePressedMap = mousePressedMap;
		this->mouseReleasedMap = mouseReleasedMap;
		this->mouseMovedEvent = mouseMoved;
		this->touchPressedEvent = touchPressed;
		this->touchReleasedEvent = touchReleased;
		this->touchMovedEvent = touchMoved;
	}
	virtual ~InputComponent() {}

	bool keyPressed(const OIS::KeyEvent &event)
	{
		for (std::pair<OIS::KeyCode, std::string> input : keyPressedMap)
		{
			if (event.key == input.first)
			{
				GameEventPtr inputEvent(new InputEvent(input.second, this->entityID));
				safeQueueEvent(inputEvent);
			}
		}
		return true;
	}

	bool keyReleased(const OIS::KeyEvent &event)
	{
		for (std::pair<OIS::KeyCode, std::string> input : keyReleasedMap)
		{
			if (event.key == input.first)
			{
				GameEventPtr inputEvent(new InputEvent(input.second, this->entityID));
				safeQueueEvent(inputEvent);
			}
		}

		return true;
	}

	bool mousePressed(const OIS::MouseEvent &event, OIS::MouseButtonID button)
	{
		for (std::pair<OIS::MouseButtonID, std::string> input : mousePressedMap)
		{
			if (button == input.first)
			{
				GameEventPtr inputEvent(new InputEvent(input.second, this->entityID, event.state.X.abs, event.state.Y.abs));
				safeQueueEvent(inputEvent);
			}
		}

		return true;
	}

	bool mouseReleased(const OIS::MouseEvent &event, OIS::MouseButtonID button)
	{
		for (std::pair<OIS::MouseButtonID, std::string> input : mouseReleasedMap)
		{
			if (button == input.first)
			{
				GameEventPtr inputEvent(new InputEvent(input.second, this->entityID, event.state.X.abs, event.state.Y.abs));
				safeQueueEvent(inputEvent);
			}
		}

		return true;
	}

	bool mouseMoved(const OIS::MouseEvent &event)
	{

		GameEventPtr inputEvent(new InputEvent(mouseMovedEvent, this->entityID, event.state.X.abs, event.state.Y.abs));
		safeQueueEvent(inputEvent);

		return true;
	}

	bool touchPressed(const OIS::MultiTouchEvent &event)
	{
		GameEventPtr inputEvent(new InputEvent(touchPressedEvent, this->entityID, event.state.X.abs, event.state.Y.abs));
		safeQueueEvent(inputEvent);

		return true;
	}

	bool touchReleased(const OIS::MultiTouchEvent &event)
	{

		GameEventPtr inputEvent(new InputEvent(touchReleasedEvent, this->entityID, event.state.X.abs, event.state.Y.abs));
		safeQueueEvent(inputEvent);

		return true;
	}

	bool touchMoved(const OIS::MultiTouchEvent &event)
	{

		GameEventPtr inputEvent(new InputEvent(touchMovedEvent, this->entityID, event.state.X.abs, event.state.Y.abs));
		safeQueueEvent(inputEvent);

		return true;
	}

	std::map<OIS::KeyCode, std::string> keyReleasedMap;
	std::map<OIS::KeyCode, std::string> keyPressedMap;
	std::map<OIS::MouseButtonID, std::string> mousePressedMap;
	std::map<OIS::MouseButtonID, std::string> mouseReleasedMap;
	std::string mouseMovedEvent;
	std::string touchPressedEvent;
	std::string touchReleasedEvent;
	std::string touchMovedEvent;
};
