#pragma once

#include "PhysXFrameworkPX_Includes.h"
#include "Core/Math/Module.h"



FORCEINLINE FVector2& physx_cast(const physx::PxVec2& obj)
{
	return *(FVector2*)&obj;
}



FORCEINLINE FVector3& physx_cast(const physx::PxVec3& obj)
{
	return *(FVector3*)&obj;
}




FORCEINLINE FVector4& physx_cast(const physx::PxVec4& obj)
{
	return *(FVector4*)&obj;
}




FORCEINLINE FQuaternion physx_cast(const physx::PxQuat& obj)
{
	/*
	FQuaternion q;
	q.x = obj.z;
	q.y = obj.y;
	q.z = obj.x;
	q.w = obj.w;*/

	return FQuaternion(obj.w, obj.x, obj.y, obj.z);
}




FORCEINLINE FMatrix4& physx_cast(const physx::PxMat44& obj)
{
	return *(FMatrix4*)&obj;
}



FORCEINLINE FMatrix3& physx_cast(const physx::PxMat33& obj)
{
	return *(FMatrix3*)&obj;
}




FORCEINLINE FTransform& physx_cast(const physx::PxTransform& obj)
{
	FTransform t;

	t.Position = physx_cast(obj.p);
	t.Orientation = physx_cast(obj.q);

	return t;
}



// ============================== FLAME TO PX ===========================



FORCEINLINE physx::PxVec2& physx_cast(const FVector2& obj)
{
	return *(physx::PxVec2*)&obj;
}



FORCEINLINE physx::PxVec3& physx_cast(const FVector3& obj)
{
	return *(physx::PxVec3*)&obj;
}




FORCEINLINE physx::PxVec4& physx_cast(const FVector4& obj)
{
	return *(physx::PxVec4*)&obj;
}





FORCEINLINE physx::PxQuat physx_cast(const FQuaternion& obj)
{
	return physx::PxQuat(obj.x, obj.y, obj.z, obj.w);
}




FORCEINLINE physx::PxMat44& physx_cast(const FMatrix4& obj)
{
	return *(physx::PxMat44*)&obj;
}



FORCEINLINE physx::PxMat33& physx_cast(const FMatrix3& obj)
{
	return *(physx::PxMat33*)&obj;
}




FORCEINLINE const physx::PxTransform& physx_cast(const FTransform& obj)
{
	return physx::PxTransform(physx_cast(obj.Position), physx_cast(obj.Orientation));
}

