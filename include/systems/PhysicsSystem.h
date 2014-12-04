#pragma once

#include <TransformMotionState.h>
#include <typeinfo>

#include "btBulletDynamicsCommon.h"

#include "System.h"
#include "PhysicsDebugDraw.h"

class PhysicsSystem: public System, public IEventListener
{
public:
	void start();
	void update(float deltaTime);
	void stop();

	std::string getName() { return "PhysicsSystemListener"; }
	bool handleEvent(const GameEvent& event);
private:
	void initPhysicsComponent(std::shared_ptr<Component> component);
	void removePhysicsComponent(std::shared_ptr<Component> component);
	static void bulletInternalTickCallback(btDynamicsWorld* world, const btScalar timeStep);

	btDefaultCollisionConfiguration* collisionConfiguration;
	btCollisionDispatcher* dispatcher;
	btBroadphaseInterface* broadphase;
	btSequentialImpulseConstraintSolver* solver;
	btDiscreteDynamicsWorld* dynamicsWorld;
	PhysicsDebugDraw* debugDrawer;

	std::map<int, btCollisionShape*> collisionShapes;

	typedef std::pair<const btCollisionObject*, const btCollisionObject*> CollisionPair;
	typedef std::set<CollisionPair> CollisionPairs;
	CollisionPairs previousTickCollisionPairs;
};
