#pragma once

#include "../EntityComponent/EntityComponent.h"
#include "Core/Math/Module.h"
#include "Core/Engine/Common/FTime.h"

#include "PhysicsShape.h"


EXPORT(struct, StaticRigidBody)
{
private:
	class FPXAllocator* Allocator;

	class PhysXActorProxy;
	PhysXActorProxy* pPxActor;
	friend class FPXAllocator;
	StaticRigidBody(PhysXActorProxy * pActor, class FPXAllocator* allocator) : pPxActor(pActor), Allocator(allocator) {}
public:

	void SetShape(const PhysShape & physShape);
	void RemoveShapes();
	void SetGlobalTransform(FTransform transform);
	FTransform GetGlobalTransform() const;

	bool IsAwake() const;
	bool HasPhysics() const;
	void Release();

};