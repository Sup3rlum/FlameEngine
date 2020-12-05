#pragma once

#include "../../dll/nchfx.h"

#include "../../Engine/FrameTime.h"
#include "../../Context/ContextDescription.h"
#include "../../Context/Context.h"
#include "../../Mathematics/Module.h"


EXPORT(class,  Camera)
{

public:
	Camera();
	~Camera();


	virtual void GetFrustumCorners(FVector3* corners);

	FMatrix4 View;
	FMatrix4 DebugView;
	FMatrix4 Projection;
	FMatrix4 ProjectionInverse;
	FMatrix4 ViewInverse;

	FVector3 Position;
	FVector3 LookDirection;
	FVector3 Right;
	FVector3 Up;

	float nearClip;
	float farClip;

	virtual void Update();


};

