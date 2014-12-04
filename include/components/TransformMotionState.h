#pragma once

#include <Ogre.h>
#include <btBulletDynamicsCommon.h>

#include "Component.h"

/*
 * This class handles the synchronization of a TransformComponent and
 * the state of the corresponding bullet physics object.
 */
class TransformMotionState : public btMotionState
{
protected:
    TransformComponent* transform;

public:
    TransformMotionState(TransformComponent* transform)
	{
		this->transform = transform;
	}

    virtual ~TransformMotionState()
    {
    }

    virtual void getWorldTransform(btTransform &worldTrans) const
    {
        worldTrans = btTransform(btQuaternion(transform->getRotation().x, transform->getRotation().y, transform->getRotation().z, transform->getRotation().w), btVector3(transform->getPosition().x, transform->getPosition().y, transform->getPosition().z));
    }

    virtual void setWorldTransform(const btTransform &worldTrans)
    {
        btQuaternion rot = worldTrans.getRotation();
        transform->setRotation(Ogre::Quaternion(rot.w(), rot.x(), rot.y(), rot.z()));
        btVector3 pos = worldTrans.getOrigin();
        transform->setPosition(Ogre::Vector3(pos.x(), pos.y(), pos.z()));
    }
};
