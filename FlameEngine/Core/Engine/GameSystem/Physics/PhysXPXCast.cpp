#include "PhysXPXCast.h"



template<>
FVector2& physx_cast<physx::PxVec2, FVector2>(const physx::PxVec2& obj)
{
	return *(FVector2*)&obj;
}


template<>
FVector3& physx_cast<physx::PxVec3, FVector3>(const physx::PxVec3& obj)
{
	return *(FVector3*)&obj;
}



template<>
FVector4& physx_cast<physx::PxVec4, FVector4>(const physx::PxVec4& obj)
{
	return *(FVector4*)&obj;
}



template<>
FQuaternion& physx_cast<physx::PxQuat, FQuaternion>(const physx::PxQuat& obj)
{
	FQuaternion q;
	q.x = obj.z;
	q.y = obj.y;
	q.z = obj.x;
	q.w = obj.w;

	return q;
}



template<>
FMatrix4& physx_cast<physx::PxMat44, FMatrix4>(const physx::PxMat44& obj)
{
	return *(FMatrix4*)&obj;
}


template<>
FMatrix3& physx_cast<physx::PxMat33, FMatrix3>(const physx::PxMat33& obj)
{
	return *(FMatrix3*)&obj;
}



template<>
FTransform& physx_cast<physx::PxTransform, FTransform>(const physx::PxTransform& obj)
{
	FTransform t;

	t.Position = physx_cast<physx::PxVec3, FVector3>(obj.p);
	t.Orientation = physx_cast<physx::PxQuat, FQuaternion>(obj.q);

	return t;
}

