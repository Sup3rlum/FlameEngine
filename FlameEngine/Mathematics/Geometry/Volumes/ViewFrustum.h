#pragma once


#include "../../../dll/nchfx.h"
#include "../../Types/fMatrix4.h"
#include "../fPlane.h"


EXPORT(class, ViewFrustum)
{
public:
	ViewFrustum(fMatrix4 vp);

	static void GetCorners(ViewFrustum vf, fVector3* corners);
	static void GetPlanes(ViewFrustum vf, fPlane(&planes)[6]);

private:
	fMatrix4 viewProjection;
	fPlane top, bottom, left, right, znear, zfar;
};

