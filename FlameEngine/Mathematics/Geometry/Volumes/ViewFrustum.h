#pragma once


#include "../../../dll/nchfx.h"
#include "../../Types/FMatrix4.h"
#include "../fPlane.h"


EXPORT(class, ViewFrustum)
{
public:
	ViewFrustum(FMatrix4 vp);

	static void GetCorners(ViewFrustum vf, FVector3* corners);
	static void GetPlanes(ViewFrustum vf, fPlane(&planes)[6]);

private:
	FMatrix4 viewProjection;
	fPlane top, bottom, left, right, znear, zfar;
};

