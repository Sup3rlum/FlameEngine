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
	CapsControllerDesc.height = 1.0f;
	CapsControllerDesc.invisibleWallHeight = 0;
	CapsControllerDesc.material = fpxService->mPxPhysics->createMaterial(0.5, 0.5, 0.5);
	CapsControllerDesc.maxJumpHeight = 0;
	CapsControllerDesc.nonWalkableMode = PxControllerNonWalkableMode::ePREVENT_CLIMBING_AND_FORCE_SLIDING;
	CapsControllerDesc.position = PxExtendedVec3(vec.x, vec.y, vec.z);
	CapsControllerDesc.radius = 0.4f;
	CapsControllerDesc.scaleCoeff = 0.9f;
	CapsControllerDesc.slopeLimit = cosf(3.1415f / 3.0f);
	CapsControllerDesc.stepOffset = 0.1f;
	CapsControllerDesc.upDirection = PxVec3(0, 1, 0);
	CapsControllerDesc.volumeGrowth = 1.5f;


	auto controllerPtr = fpxScene->mPxControllerManager->createController(CapsControllerDesc);

	return CharacterBody(static_cast<CharacterBody::PhysXControllerProxy*>(controllerPtr), this);
}

VehicleBody FPXAllocator::CreateVehicle(FTransform transform)
{
/*	const PxU32 numWheels = 4;

	PxVehicleWheelsSimData* wheelsSimData = PxVehicleWheelsSimData::allocate(numWheels);
	setupWheelsSimulationData(wheelsSimData);

	PxVehicleDriveSimData4W driveSimData;
	setupDriveSimData(driveSimData);

	PxRigidDynamic* vehActor = myPhysics.createRigidDynamic(startPose);
	setupVehicleActor(vehActor);
	myScene.addActor(*vehActor);

	PxVehicleDrive4W* vehDrive4W = PxVehicleDrive4W::allocate(numWheels);
	vehDrive4W->setup(physics, veh4WActor, *wheelsSimData, driveSimData, numWheels - 4);
	wheelsSimData->free();*/

	return VehicleBody(NULL, NULL);
}

TriangleMeshGeometry FPXAllocator::CookTriangleMeshGeometry(const PhysicsTriangleMeshDesc& desc)
{
	PxTriangleMeshDesc TrMeshDesc;

	TrMeshDesc.points.count = desc.VertexData.ByteSize / desc.DataStride;
	TrMeshDesc.points.stride = desc.DataStride;
	TrMeshDesc.points.data = desc.VertexData.DataArray;

	TrMeshDesc.triangles.count = desc.IndexData.ByteSize / 12;
	TrMeshDesc.triangles.stride = 3 * sizeof(uint32);
	TrMeshDesc.triangles.data = desc.IndexData.DataArray;

	PxDefaultMemoryOutputStream outStream;
	PxTriangleMeshCookingResult::Enum result;

	bool status = fpxService->mPxCooking->cookTriangleMesh(TrMeshDesc, outStream, &result);

	PxDefaultMemoryInputData readBuffer(outStream.getData(), outStream.getSize());
	PxGeometry* pxPtr = new PxTriangleMeshGeometry(fpxService->mPxPhysics->createTriangleMesh(readBuffer));

	TriangleMeshGeometry trMesh;
	trMesh.FPXGeometry = static_cast<PhysicsGeometry::FPXGeometryProxy*>(pxPtr);

	return trMesh;
}

bool FPXAllocator::RayCast(FRay ray, float distance, FVector3& outPosition, FVector3& outNormal)
{
	PxVec3 downDir(0, -1, 0);
	PxRaycastBuffer hit;

	bool status = fpxScene->sceneHandle->raycast(
		physx_cast(ray.Origin),
		physx_cast(ray.Direction),
		distance,
		hit
	);

	if (status)
	{
		outPosition = physx_cast(hit.block.position);
		outNormal = physx_cast(hit.block.normal);
	}

	return status;
}