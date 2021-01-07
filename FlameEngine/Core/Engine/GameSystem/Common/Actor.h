#pragma once


#include "Core/Common/CoreCommon.h"

#include "EntityBase.h"
#include "Core/Math/Module.h"
#include "Core/Engine/FlameRI/FRICommandList.h"


EXPORT_FORWARD_DECL(class, FEntityGroup);


EXPORT(class,  FActor) : public FSceneEntity<FActor>
{


	class PhysXRigidDynamicProxy;

	FUniquePointer<PhysXRigidDynamicProxy> pPxActor;

public:
	

	FActor() : FSceneEntity<FActor>("") {}

	void SetLinearVelocity(FVector3 velocity);
	void SetAngularVelocity(FVector3 avelocity);

	void SetPosition(FVector3 position);
	void SetOrientation(FQuaternion orientation);

	void AddForce(FVector3 force);
	void AddTorque(FVector3 force);

	void ClearForce();
	void ClearTorque();


	void SetGlobalTransform(FTransform transform);
	void SetRelativeTransform(FActor& relative, FTransform transform);


	bool IsAwake() const;
	bool HasPhysics() const;

protected:
	FEntityGroup* parentGroup = NULL;

};

