#pragma once


#include "Scene.h"

struct DynamicPhys : public FEntityComponentSystem<FTransform, RigidBody>
{
	void Update(Entity, FTransform&, RigidBody&);
};


struct AnimSystem : public FEntityComponentSystem<SkinnedMeshComponent, AnimationComponent>
{
	void Update(Entity, SkinnedMeshComponent&, AnimationComponent&);
};