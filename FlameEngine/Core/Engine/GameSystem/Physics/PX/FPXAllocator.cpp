#include "FPXService.h"
#include "FPXAllocator.h"
#include "FPXActorProxy.h"
#include "FPXScene.h"

#include "../PhysicsGeometryProxy.h"

RigidBody FPXAllocator::CreateDynamic(FTransform transform)
{
	PxVec3 vec = physx_cast(transform.Position);
	PxQuat quat = physx_cast(transform.Orientation);
	PxTransform tr(vec, quat);

	auto bodyPtr = fpxService->mPxPhysics->createRigidDynamic(tr);

	fpxScene->sceneHandle->addActor(*bodyPtr);

	return RigidBody(static_cast<RigidBody::PhysXActorProxy*>(bodyPtr), this);
}


StaticRigidBody FPXAllocator::CreateStatic(FTransform transform)
{
	PxVec3 vec = physx_cast(transform.Position);
	PxQuat quat = physx_cast(transform.Orientation);
	PxTransform tr(vec, quat);


	auto bodyPtr = fpxService->mPxPhysics->createRigidStatic(tr);

	fpxScene->sceneHandle->addActor(*bodyPtr);

	return StaticRigidBody(static_cast<StaticRigidBody::PhysXActorProxy*>(bodyPtr), this);
}

CharacterBody FPXAllocator::CreateCharacter(FTransform transform)
{
	PxVec3 vec = physx_cast(transform.Position);
	PxQuat quat = physx_cast(transform.Orientation);
	PxTransform tr(vec, quat);

	PxCapsuleControllerDesc CapsControllerDesc;
	CapsControllerDesc.climbingMode = PxCapsuleClimbingMode::eCONSTRAINED;
	CapsControllerDesc.contactOffset = 0.1f;
	CapsControllerDesc.density = 10.0f;
	CapsControllerDesc.height = 2.0f;
	CapsControllerDesc.invisibleWallHeight = 0;
	CapsControllerDesc.material = fpxService->mPxPhysics->createMaterial(0.5, 0.5, 0.5);
	CapsControllerDesc.maxJumpHeight = 0;
	CapsControllerDesc.nonWalkableMode = PxControllerNonWalkableMode::ePREVENT_CLIMBING_AND_FORCE_SLIDING;
	CapsControllerDesc.position = PxExtendedVec3(vec.x, vec.y, vec.z);
	CapsControllerDesc.radius = 1.0f;
	CapsControllerDesc.scaleCoeff = 0.9f;
	CapsControllerDesc.slopeLimit = cosf(3.1415f / 3.0f);
	CapsControllerDesc.stepOffset = 0.1f;
	CapsControllerDesc.upDirection = PxVec3(0, 1, 0);
	CapsControllerDesc.volumeGrowth = 1.5f;


	auto controllerPtr = fpxScene->mPxControllerManager->createController(CapsControllerDesc);

	return CharacterBody(static_cast<CharacterBody::PhysXControllerProxy*>(controllerPtr), this);
}



TriangleMeshGeometry FPXAllocator::CookTriangleMeshGeometry(const PhysicsTriangleMeshDesc& desc)
{
	PxTriangleMeshDesc TrMeshDesc;

	TrMeshDesc.points.count = desc.VertexData.Length() / desc.DataStride;
	TrMeshDesc.points.stride = desc.DataStride;
	TrMeshDesc.points.data = &desc.VertexData[0];

	TrMeshDesc.triangles.count = desc.IndexData.Length() / 3;
	TrMeshDesc.triangles.stride = 3 * sizeof(uint32);
	TrMeshDesc.triangles.data = &desc.IndexData[0];

	PxDefaultMemoryOutputStream outStream;
	PxTriangleMeshCookingResult::Enum result;

	bool status = fpxService->mPxCooking->cookTriangleMesh(TrMeshDesc, outStream, &result);

	PxDefaultMemoryInputData readBuffer(outStream.getData(), outStream.getSize());
	PxGeometry* pxPtr = new PxTriangleMeshGeometry(fpxService->mPxPhysics->createTriangleMesh(readBuffer));

	TriangleMeshGeometry trMesh;
	trMesh.FPXGeometry = static_cast<PhysicsGeometry::FPXGeometryProxy*>(pxPtr);

	return trMesh;
}