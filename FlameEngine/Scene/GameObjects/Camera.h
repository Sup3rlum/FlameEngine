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


	virtual void GetFrustumCorners(fVector3* corners);

	fMatrix4 View;
	fMatrix4 DebugView;
	fMatrix4 Projection;
	fMatrix4 ProjectionInverse;
	fMatrix4 ViewInverse;

	fVector3 Position;
	fVector3 LookDirection;
	fVector3 Right;
	fVector3 Up;

	float nearClip;
	float farClip;

	virtual void Update();


};

