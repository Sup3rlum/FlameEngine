#include "SceneSystems.h"

void DynamicPhys::Update(Entity ent, FTransform& transform, RigidBody& physBody)
{
	transform = physBody.GetGlobalTransform();
}

void AnimSystem::Update(Entity ent, SkinnedMesh& mesh, AnimationComponent& anim)
{
	anim.Step(1.0f / 10.0f);

	if (anim.CurrentSequenceRef)
	{
		mesh.MeshSkeleton.ApplyPose(anim.CurrentAnimationPose);
	}
}

void BehaviourSystem::Update(Entity ent, Behaviour& behaviour)
{
	if (behaviour.pScript)
	{
		behaviour.pScript->Update();
	}
}
