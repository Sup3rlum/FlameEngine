#pragma once


#include "Core/Math/Module.h"
#include "../EntityComponent/EntityComponent.h"


EXPORT(struct,  CameraComponent)
{

public:
	CameraComponent(FViewMatrix viewmat, FProjectionMatrix perspmat) :  
		View(viewmat),
		Projection(perspmat)
	{}


	CameraComponent(FProjectionMatrix perspmat) :

		View(FMatrix4::Identity()),
		Projection(perspmat)
	{}

	
	void GetFrustumCorners(FStaticArray<FVector3, 8>& corners);

	FViewMatrix View;
	FProjectionMatrix Projection;
};


//RegisterEntityComponent(CameraComponent, "CameraComponent");