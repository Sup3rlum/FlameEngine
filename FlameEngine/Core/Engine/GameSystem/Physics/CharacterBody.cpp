#include "CharacterBody.h"

#include "PX/FPXActorProxy.h"
#include "PX/FPXAllocator.h"
#include "PX/FPXService.h"
#include "PX/FPXScene.h"
#include "PhysicsGeometryProxy.h"


void CharacterBody::Move(FVector3 vec)
{
	auto MoveTimestamp = FTime::GetTimestamp();
	auto MoveTimeDelta = (MoveTimestamp - LastMoveTimestamp).GetSeconds();
	LastMoveTimestamp = MoveTimestamp;

	if (MoveTimeDelta < 2.0f)
	{
		vec *= MoveTimeDelta;
		pPxController->move(physx_cast(vec), 0.005f, MoveTimeDelta, PxControllerFilters());


		auto raycastOrigin = pPxController->getFootPosition();
		PxVec3 downDir(0, -1, 0);
		PxRaycastBuffer hit;

		bool status = Allocator->fpxScene->sceneHandle->raycast(
			PxVec3(raycastOrigin.x, raycastOrigin.y + 0.1f, raycastOrigin.z),
			downDir,
			0.2f,
			hit
		);

		isGrounded =
			status &&
			hit.block.normal.dot(PxVec3(0, 1, 0)) > 0.7f;
	}
}


FTransform CharacterBody::GetGlobalTransform() const
{
	PxExtendedVec3 pos = pPxController->getFootPosition();
	return FTransform(FVector3(pos.x, pos.y, pos.z));
}


FVector3 CharacterBody::GetViewpoint(float f) const
{
	auto height = GetHeight() + 2 * pPxController->getRadius();
	auto position = GetGlobalTransform().Position;

	return position + FVector3(0.0f, height * f, 0.0f);
}

bool CharacterBody::IsGrounded() const
{
	return isGrounded;
}

float CharacterBody::GetHeight() const
{
	return pPxController->getHeight();
}

void CharacterBody::Release()
{
	pPxController->release();
}