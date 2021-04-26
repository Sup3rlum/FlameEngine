#pragma once


#include "../PhysicsComponent.h"
#include "PhysXPXCast.h"
#include "FPXShape.h"

using namespace physx;

class DynamicPhysicsComponent::PhysXActorProxy : public PxRigidDynamic
{
};

class StaticPhysicsComponent::PhysXActorProxy : public PxRigidStatic
{
};
