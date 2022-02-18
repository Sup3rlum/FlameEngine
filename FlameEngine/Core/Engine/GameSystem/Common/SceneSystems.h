#pragma once


#include "Scene.h"
#include "../Behaviour.h"

struct DynamicPhys : public FEntityComponentSystem<FTransform, RigidBody>
{
	void Update(Entity, FTransform&, RigidBody&);
};


struct AnimSystem : public FEntityComponentSystem<SkinnedMesh, AnimationComponent>
{
	void Update(Entity, SkinnedMesh&, AnimationComponent&);
};

struct BehaviourSystem : public FEntityComponentSystem<Behaviour>
{
	void Update(Entity, Behaviour&);
};

