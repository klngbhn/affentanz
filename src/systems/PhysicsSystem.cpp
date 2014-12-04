#include "PhysicsSystem.h"

void PhysicsSystem::start()
{
	safeAddListener(dynamic_cast<IEventListener*>(this), EventType("ComponentAdded"));
	safeAddListener(dynamic_cast<IEventListener*>(this), EventType("EntityDestroyed"));
	safeAddListener(dynamic_cast<IEventListener*>(this), EventType("RemoveComponent"));
	safeAddListener(dynamic_cast<IEventListener*>(this), EventType("ShowDebugPhysics"));
	safeAddListener(dynamic_cast<IEventListener*>(this), EventType("AddForce"));

	collisionConfiguration = new btDefaultCollisionConfiguration();
	dispatcher = new btCollisionDispatcher(collisionConfiguration);
	broadphase = new btDbvtBroadphase();
	solver = new btSequentialImpulseConstraintSolver;

	dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher,broadphase,solver,collisionConfiguration);
	dynamicsWorld->setGravity(btVector3(0,-10,0));

	debugDrawer = new PhysicsDebugDraw(sceneMgr, dynamicsWorld);

	dynamicsWorld->setInternalTickCallback(bulletInternalTickCallback);
	dynamicsWorld->setWorldUserInfo(this);

	LOG_DEBUG("Physics system started");
}

void PhysicsSystem::update(float deltaTime)
{
	if (dynamicsWorld)
		dynamicsWorld->stepSimulation(deltaTime, 10);
	if (debugDrawer)
		debugDrawer->update();
}

void PhysicsSystem::stop()
{
	safeDelListener(dynamic_cast<IEventListener*>(this), EventType("ComponentAdded"));
	safeDelListener(dynamic_cast<IEventListener*>(this), EventType("EntityDestroyed"));
	safeDelListener(dynamic_cast<IEventListener*>(this), EventType("RemoveComponent"));
	safeDelListener(dynamic_cast<IEventListener*>(this), EventType("ShowDebugPhysics"));
	safeDelListener(dynamic_cast<IEventListener*>(this), EventType("AddForce"));

	// Remove rigid bodies from the dynamics world and delete them
	for (int i = dynamicsWorld->getNumCollisionObjects()-1; i >= 0 ; i--)
	{
		btCollisionObject* obj = dynamicsWorld->getCollisionObjectArray()[i];
		btRigidBody* body = btRigidBody::upcast(obj);
		if (body && body->getMotionState())
		{
			delete body->getMotionState();
		}
		dynamicsWorld->removeCollisionObject( obj );
		delete obj;
	}

	// Delete collision shapes
	for(std::pair<int, btCollisionShape*> entry : collisionShapes)
	{
		btCollisionShape* shape = collisionShapes[entry.first];
		collisionShapes[entry.first] = 0;
		delete shape;
	}

	delete debugDrawer;
	delete dynamicsWorld;
	delete solver;
	delete broadphase;
	delete dispatcher;
	delete collisionConfiguration;

	collisionShapes.clear();

	LOG_DEBUG("Physics system stopped");
}

void PhysicsSystem::bulletInternalTickCallback(btDynamicsWorld* world, const btScalar timeStep)
{
	// Get physics system
	PhysicsSystem* physicsSystem = static_cast<PhysicsSystem*>(world->getWorldUserInfo());

	// Get manifolds (collisions between two objects)
	int numManifolds = world->getDispatcher()->getNumManifolds();
	CollisionPairs currentTickCollisionPairs;

	for (int i = 0; i < numManifolds; i++)
	{
		// Get collision pair objects
		btPersistentManifold* contactManifold =  world->getDispatcher()->getManifoldByIndexInternal(i);
		const btCollisionObject* obA = static_cast<const btCollisionObject*>(contactManifold->getBody0());
		const btCollisionObject* obB = static_cast<const btCollisionObject*>(contactManifold->getBody1());

		// Sort collision pair objects
		bool const swapped = obA > obB;
		const btCollisionObject* sortedObA = swapped ? obB : obA;
		const btCollisionObject* sortedObB = swapped ? obA : obB;

		const CollisionPair thisPair = std::make_pair(sortedObA, sortedObB);
		currentTickCollisionPairs.insert(thisPair);

		if (physicsSystem->previousTickCollisionPairs.find(thisPair) == physicsSystem->previousTickCollisionPairs.end())
		{
			// Send event because this is a new collision
			GameEventPtr event(new CollisionEvent("EnteredCollision", sortedObA->getUserIndex(), sortedObB->getUserIndex()));
			safeQueueEvent(event);
		}
	}

	// Get removed collisions
	CollisionPairs removedCollisionPairs;
	std::set_difference(physicsSystem->previousTickCollisionPairs.begin(), physicsSystem->previousTickCollisionPairs.end(),
						currentTickCollisionPairs.begin(), currentTickCollisionPairs.end(), std::inserter(removedCollisionPairs, removedCollisionPairs.begin()));

	for (CollisionPair collisionPair : removedCollisionPairs)
	{
		// Send event because this collision was removed
		GameEventPtr event(new CollisionEvent("LeavedCollision", collisionPair.first->getUserIndex(), collisionPair.second->getUserIndex()));
		safeQueueEvent(event);
	}

	// Save collision pairs for the next round
	physicsSystem->previousTickCollisionPairs = currentTickCollisionPairs;
}

bool PhysicsSystem::handleEvent(const GameEvent& event)
{
	if (event.getEventType() == EventType("ComponentAdded"))
	{
		const ComponentEvent& componentEvent = dynamic_cast<const ComponentEvent&>(event);
		if (componentEvent.componentType == &typeid(PhysicsComponent))
			initPhysicsComponent(componentMgr->getComponent(componentEvent.entityID, &typeid(PhysicsComponent)));
	}
	else if (event.getEventType() == EventType("EntityDestroyed"))
	{
		const EntityEvent& entityEvent = dynamic_cast<const EntityEvent&>(event);
		if (componentMgr->getComponent(entityEvent.entityID, &typeid(PhysicsComponent)))
			removePhysicsComponent(componentMgr->getComponent(entityEvent.entityID, &typeid(PhysicsComponent)));
	}
	else if (event.getEventType() == EventType("RemoveComponent"))
	{
		const ComponentEvent& componentEvent = dynamic_cast<const ComponentEvent&>(event);
		if (componentEvent.componentType == &typeid(PhysicsComponent))
			removePhysicsComponent(componentMgr->getComponent(componentEvent.entityID, &typeid(PhysicsComponent)));
	}
	else if (event.getEventType() == EventType("ShowDebugPhysics"))
	{
		debugDrawer->setActive(!debugDrawer->isActive());
	}
	else if (event.getEventType() == EventType("AddForce"))
	{
		const ForceEvent& forceEvent = dynamic_cast<const ForceEvent&>(event);
		PhysicsComponent* physicsComponent = dynamic_cast<PhysicsComponent*>(componentMgr->getComponent(forceEvent.entityID, &typeid(PhysicsComponent)).get());
		physicsComponent->body->setLinearFactor(btVector3(1,1,1));
		btVector3 linVel(forceEvent.directionX, forceEvent.directionY, forceEvent.directionZ);
		linVel.normalize();
		linVel*=forceEvent.velocity;

		physicsComponent->body->setLinearVelocity(linVel);
		physicsComponent->body->setAngularVelocity(btVector3(0,0,0));
		physicsComponent->body->setCcdMotionThreshold(0.5);
		physicsComponent->body->setCcdSweptSphereRadius(0.4f);
	}

	return true;
}

void PhysicsSystem::initPhysicsComponent(std::shared_ptr<Component> component)
{
	PhysicsComponent* physicsComponent = dynamic_cast<PhysicsComponent*>(component.get());
	TransformComponent* transform = dynamic_cast<TransformComponent*>(componentMgr->getComponent(physicsComponent->entityID, &typeid(TransformComponent)).get());

	btCollisionShape* collisionShape;

	switch (physicsComponent->shape)
	{
	case CollisionShape::Box:
		collisionShape = new btBoxShape(physicsComponent->dimensions);
		break;
	case CollisionShape::Sphere:
		collisionShape = new btSphereShape(physicsComponent->radius);
		break;
	case CollisionShape::Capsule:
		collisionShape = new btCapsuleShape(physicsComponent->radius, physicsComponent->height);
		break;
	}

//	TODO: Create collision shape from point cloud
//	const btConvexHullShape* shape = new btConvexHullShape();
//	for (int i = 0; i < numPoints; ++i)
//		shape->addPoint(vertices[i]);

	collisionShapes[physicsComponent->entityID] = collisionShape;

	bool isDynamic = (physicsComponent->mass != 0.f);
	if (isDynamic)
		collisionShape->calculateLocalInertia(physicsComponent->mass, physicsComponent->localInertia);

	TransformMotionState* motionState = new TransformMotionState(transform);
	btRigidBody::btRigidBodyConstructionInfo rbInfo(physicsComponent->mass, motionState, collisionShape, physicsComponent->localInertia);
	btRigidBody* body = new btRigidBody(rbInfo);
	body->setUserIndex(physicsComponent->entityID);
	physicsComponent->body = body;

	dynamicsWorld->addRigidBody(body);

	if (physicsComponent->isTrigger)
		body->setCollisionFlags(body->getCollisionFlags() | btRigidBody::CF_NO_CONTACT_RESPONSE);
}

void PhysicsSystem::removePhysicsComponent(std::shared_ptr<Component> component)
{
	PhysicsComponent* physicsComponent = dynamic_cast<PhysicsComponent*>(component.get());
	btRigidBody* body = physicsComponent->body;

	if (body && body->getMotionState())
	{
		delete body->getMotionState();
	}
	if (body && body->getCollisionShape())
	{
		collisionShapes[physicsComponent->entityID] = 0;
		delete body->getCollisionShape();
	}
	dynamicsWorld->removeCollisionObject(body);
	delete body;

	componentMgr->removeComponent(physicsComponent->entityID, &typeid(PhysicsComponent));
}
