#pragma once


#include "Core/Math/Module.h"
#include "../EntityComponent/EntityComponent.h"


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

	FViewMatrix View;
	FProjectionMatrix Projection;
};


//RegisterEntityComponent(CameraComponent, "CameraComponent");