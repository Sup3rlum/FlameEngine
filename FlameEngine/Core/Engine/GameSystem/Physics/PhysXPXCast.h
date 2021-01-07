#pragma once


#include "PhysXFrameworkPX_Includes.h"


#include "Core/Math/Module.h"



/*
* 
*	Since PhysX has its own math types/system we need our 
*	proxies and services to cast quickly to and from our own types 
* 
*/


template<typename TFrom, typename TTo>
TTo& physx_cast(const TFrom& obj)
{
	return *static_cast<TTo*>(&obj);
}



#define REGISTER_FROM_PX_CAST(pxtype, fltype) template<> \
			FORCEINLINE fltype& physx_cast<pxtype, fltype>(const pxtype& obj); \

REGISTER_FROM_PX_CAST(physx::PxVec3, FVector3)
REGISTER_FROM_PX_CAST(physx::PxVec2, FVector2)
REGISTER_FROM_PX_CAST(physx::PxVec4, FVector4)
REGISTER_FROM_PX_CAST(physx::PxQuat, FQuaternion)
REGISTER_FROM_PX_CAST(physx::PxMat44, FMatrix4)
REGISTER_FROM_PX_CAST(physx::PxMat33, FMatrix3)
REGISTER_FROM_PX_CAST(physx::PxTransform, FTransform)


REGISTER_FROM_PX_CAST(FVector3, physx::PxVec3)
REGISTER_FROM_PX_CAST(FVector2, physx::PxVec2)
REGISTER_FROM_PX_CAST(FVector4, physx::PxVec4)
REGISTER_FROM_PX_CAST(FQuaternion, physx::PxQuat)
REGISTER_FROM_PX_CAST(FMatrix4, physx::PxMat44)
REGISTER_FROM_PX_CAST(FMatrix3, physx::PxMat33)
REGISTER_FROM_PX_CAST(FTransform, physx::PxTransform)
