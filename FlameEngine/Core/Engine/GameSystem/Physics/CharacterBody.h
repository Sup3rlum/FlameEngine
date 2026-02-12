#pragma once

#include "../EntityComponent/EntityComponent.h"
#include "Core/Math/Module.h"
#include "Core/Engine/Common/FTime.h"

#include "PhysicsShape.h"

EXPORT(struct, CharacterBody)
{
private:
	class FPXAllocator* Allocator;
	class PhysXControllerProxy;
	PhysXControllerProxy* pPxController;
	friend class FPXAllocator;

	FVector3 Velocity;
	FVector3 Gravity;
	FTimeSpan LastMoveTimestamp = 0;
	bool isGrounded = false;

	CharacterBody(PhysXControllerProxy * pController, class FPXAllocator* allocator) :
		pPxController(pController),
		Allocator(allocator),
		HasGravity(true)
	{
	}
public:

	bool HasGravity;

	void Move(FVector3 Velocity);
	FTransform GetGlobalTransform() const;

	FVector3 GetViewpoint(float factor) const;

	bool IsGrounded() const;
	float GetHeight() const;

	void Release();
};