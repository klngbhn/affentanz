#include "PhysicsDebugDraw.h"

#ifndef M_PI
#define M_PI    3.14159265358979323846f
#endif

PhysicsDebugDraw::PhysicsDebugDraw(Ogre::SceneManager* scene, btDynamicsWorld* world)
: world(world),
  active(false)
{
	// generate a unit sphere
	std::size_t circleSubdivs = 12;
	unitCircle.reserve(circleSubdivs+1);

	for (std::size_t i = 0; i < circleSubdivs; ++i)
	{
		float Angle = (M_PI * 2.f * i) / circleSubdivs;
		unitCircle.push_back(Ogre::Vector2(std::cos(Angle), std::sin(Angle)));
	}
	unitCircle.push_back(unitCircle.front());

	debugLineDrawer = new CDynamicLineDrawer();
	if (!scene->hasSceneNode("DebugDrawer"))
		debugDrawerNode = scene->getRootSceneNode()->createChildSceneNode("DebugDrawer");
	else
	{
		debugDrawerNode = scene->getSceneNode("DebugDrawer");
		Ogre::SceneNode::ObjectIterator Iter = debugDrawerNode->getAttachedObjectIterator();
		while( Iter.hasMoreElements() )
			scene->destroyEntity(dynamic_cast<Ogre::Entity*>(Iter.getNext()));
	}

	debugDrawerNode->attachObject(debugLineDrawer);
	debugLineDrawer->setMaterial("DebugLines");
	world->setDebugDrawer(this);
}

PhysicsDebugDraw::~PhysicsDebugDraw()
{
	delete debugLineDrawer;
	world->setDebugDrawer(NULL);
}

void PhysicsDebugDraw::update()
{
	if (active)
	{
		world->debugDrawWorld();
		debugLineDrawer->Update();
		debugDrawerNode->needUpdate();
		debugLineDrawer->Clear();
	}
	else
	{
		debugLineDrawer->Clear();
		debugLineDrawer->Update();
		debugDrawerNode->needUpdate();
	}
}

void PhysicsDebugDraw::setDebugMode(int debugMode)
{
	this->active = debugMode!=0;

	if (!active)
	{
		debugLineDrawer->Clear();
	}
}

int PhysicsDebugDraw::getDebugMode() const
{
	return active ? 1 : 0;
}

void PhysicsDebugDraw::drawSphere(const btVector3& p, btScalar radius, const btVector3& BColor)
{
	/*Ogre::ColourValue Color( BColor[0], BColor[1], BColor[2] );

	ManualObject.begin( "", Ogre::RenderOperation::OT_LINE_LIST );
	for ( std::size_t i = 1; i < UnitCircle.size(); ++i )
	{
		Ogre::Vector2& c0 = UnitCircle[i-1];
		ManualObject.position( p[0]+c0[0]*radius, p[1]+c0[1]*radius, p[2] );
		ManualObject.colour( Color );
		Ogre::Vector2& c1 = UnitCircle[i];
		ManualObject.position( p[0]+c1[0]*radius, p[1]+c1[1]*radius, p[2] );
		ManualObject.colour( Color );
	}
	
	for ( std::size_t i = 1; i < UnitCircle.size(); ++i )
	{
		Ogre::Vector2& c0 = UnitCircle[i-1];
		ManualObject.position( p[0]+c0[0]*radius, p[1], p[2]+c0[1]*radius );
		ManualObject.colour( Color );
		Ogre::Vector2& c1 = UnitCircle[i];
		ManualObject.position( p[0]+c1[0]*radius, p[1], p[2]+c1[1]*radius );
		ManualObject.colour( Color );
	}

	for ( std::size_t i = 1; i < UnitCircle.size(); ++i )
	{
		Ogre::Vector2& c0 = UnitCircle[i-1];
		ManualObject.position( p[0], p[1]+c0[1]*radius, p[2]+c0[0]*radius );
		ManualObject.colour( Color );
		Ogre::Vector2& c1 = UnitCircle[i];
		ManualObject.position( p[0], p[1]+c1[1]*radius, p[2]+c1[0]*radius );
		ManualObject.colour( Color );
	}
	ManualObject.end();*/
}

void PhysicsDebugDraw::drawLine(const btVector3& from, const btVector3& to, const btVector3& color)
{
	debugLineDrawer->AddPoint(Ogre::Vector3(from.getX(), from.getY(), from.getZ()), Ogre::ColourValue(color.getX(), color.getY(), color.getZ()));
	debugLineDrawer->AddPoint(Ogre::Vector3(to.getX(), to.getY(), to.getZ()), Ogre::ColourValue(color.getX(), color.getY(), color.getZ()));
}


void PhysicsDebugDraw::drawTriangle(const btVector3& v0, const btVector3& v1, const btVector3& v2, const btVector3& color, btScalar alpha)
{
	/*Ogre::ColourValue OgreColor( color.x(), color.y(), color.z(), alpha );

	ManualObject.begin( "", Ogre::RenderOperation::OT_LINE_LIST );
		ManualObject.position( Ogre::Vector3( v0 ) );
		ManualObject.colour( OgreColor );
		ManualObject.position( Ogre::Vector3( v1 ) );
		ManualObject.colour( OgreColor );
		ManualObject.position( Ogre::Vector3( v1 ) );
		ManualObject.colour( OgreColor );
		ManualObject.position( Ogre::Vector3( v2 ) );
		ManualObject.colour( OgreColor );
		ManualObject.position( Ogre::Vector3( v2 ) );
		ManualObject.colour( OgreColor );
		ManualObject.position( Ogre::Vector3( v0 ) );
		ManualObject.colour( OgreColor );
	ManualObject.end();*/
}

void PhysicsDebugDraw::drawContactPoint(const btVector3& PointOnB, const btVector3& normalOnB, btScalar distance, int lifeTime, const btVector3& color)
{
}

void PhysicsDebugDraw::reportErrorWarning(const char* warningString)
{
	LOG_DEBUG(warningString);
}

void PhysicsDebugDraw::draw3dText(const btVector3& location, const char* textString)
{
}
