#pragma once


#include "Core/Math/Module.h"
#include "../EntityComponent/EntityComponent.h"
#include "../../FlameRI/FRI.h"

EXPORT(struct,  Camera)
{

public:
	Camera(FViewMatrix viewmat, FProjectionMatrix perspmat) :  
		View(viewmat),
		Projection(perspmat)
	{}


	Camera(FProjectionMatrix perspmat) :

		View(FMatrix4::Identity()),
		Projection(perspmat)
	{}

	
	void GetFrustumCorners(FStaticArray<FVector3, 8>& corners);
	void StageMemory(FRIMemoryMap& GPUMemory) const;
	float NearPlane() const;
	float FarPlane() const;
	FRay ScreenPointToRay(FVector2 screenPoint) const;

	FViewMatrix View;
	FProjectionMatrix Projection;
};


//RegisterEntityComponent(CameraComponent, "CameraComponent");