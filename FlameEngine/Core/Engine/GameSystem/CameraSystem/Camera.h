#pragma once

#include "Core/Common/CoreCommon.h"

#include "Core/Math/Module.h"
#include "../Common/EntityBase.h"

EXPORT_FORWARD_DECL(class, Camera);

EXPORT(struct, FViewFrustum)
{
	FStaticArray<FVector3, 8> Corners;
	FStaticArray<FPlane, 6> Planes;

public:
	FViewFrustum(FMatrix4 CameraMatrix);
	FViewFrustum(Camera camera);


	FStaticArray<FPlane, 6> GetPlanes() { return Planes;  }
	FStaticArray<FVector3, 8> GetCorners() { return Corners; }

};

EXPORT(struct, FCameraClipDescription)
{
	float NearClip;
	float FarClip;
};


EXPORT(class,  Camera) : public FSceneEntity<Camera>
{

public:
	Camera() : FSceneEntity<Camera>("CameraEntity") {}

	Camera(FViewMatrix viewmat, FPerspectiveMatrix perspmat) : 
		FSceneEntity<Camera>("CameraEntity"), 
		View(viewmat),
		Projection(perspmat)
	{}


	Camera(FPerspectiveMatrix perspmat) :
		FSceneEntity<Camera>("CameraEntity"),
		View(FMatrix4::Identity()),
		Projection(perspmat)
	{}

	~Camera();

	void Blend(const Camera& other, float factor);
	void Update();

	void AttachTo(FSceneEntityBase* mEntiity);

	FViewMatrix View;
	FProjectionMatrix Projection;

	FViewMatrix ViewInverse;
	FProjectionMatrix  ProjectionInverse;
	
	FViewFrustum GetFrustum() const;

	bool IsAttached() const
	{
		return mEntityAttach != NULL;
	}

protected:
	FSceneEntityBase* mEntityAttach;
	FCameraClipDescription clipDescr;
};

