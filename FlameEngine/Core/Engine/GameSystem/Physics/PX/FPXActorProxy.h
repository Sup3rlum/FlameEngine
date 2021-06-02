#pragma once


#include "../PhysicsComponent.h"
#include "PhysXPXCast.h"

using namespace physx;

class RigidBody::PhysXActorProxy : public PxRigidDynamic
{
};

class StaticRigidBody::PhysXActorProxy : public PxRigidStatic
{
};

class CharacterBody::PhysXControllerProxy : public PxCapsuleController
{

};