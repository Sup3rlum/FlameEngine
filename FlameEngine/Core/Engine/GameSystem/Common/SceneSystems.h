#pragma once


#include "Scene.h"

struct DynamicPhys : public FEntityComponentSystem<FTransform, RigidBody>
{
	void Update(Entity, FTransform&, RigidBody&);
};


struct AnimSystem : public FEntityComponentSystem<SkinnedMesh, AnimationComponent>
{
	void Update(Entity, SkinnedMesh&, AnimationComponent&);
};