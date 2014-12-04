#pragma once

#include "btBulletDynamicsCommon.h"
#include "Ogre.h"

#include "DynamicLineDrawer.hpp"
#include "Log.h"

/**
 * Implements the bullet debug draw interface for drawing with ogre. Is based on a class from:
 * http://www.ogre3d.org/forums/viewtopic.php?f=2&t=67620
*/
class PhysicsDebugDraw : public btIDebugDraw
{
public:
	PhysicsDebugDraw(Ogre::SceneManager* scene, btDynamicsWorld* world);
	~PhysicsDebugDraw();

	/**
	 * Moves the data from the world to the ogre object.
	 * Should be called once every frame.
	*/
	void update();

	void setActive(bool active) { this->active = active; }
	bool isActive() { return active; }

protected:
	void setDebugMode(int DebugMode);
	int getDebugMode() const;

	void drawLine(const btVector3& from, const btVector3& to, const btVector3& color);
	void drawContactPoint(const btVector3& PointOnB, const btVector3& normalOnB, btScalar distance, int lifeTime, const btVector3& color);
	void reportErrorWarning(const char* warningString);
	void draw3dText(const btVector3& location, const char* textString);
	void drawSphere(const btVector3& p, btScalar radius, const btVector3& color);
	void drawTriangle(const btVector3&v0, const btVector3 &v1, const btVector3 &v2, const btVector3 &color, btScalar alpha);

private:
	std::vector<Ogre::Vector2> unitCircle;

	CDynamicLineDrawer* debugLineDrawer;
	Ogre::SceneNode*	debugDrawerNode;
	btDynamicsWorld*	world;
	bool				active;
};
