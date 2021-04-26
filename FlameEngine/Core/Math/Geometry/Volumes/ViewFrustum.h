#pragma once


#include "Core/Common/CoreCommon.h"
#include "../../Types/Matrix.h"
#include "../FPlane.h"


EXPORT(class, ViewFrustum)
{
public:
	ViewFrustum(const FMatrix4& vp);

	static void GetCorners(ViewFrustum vf, FVector3* corners);
	static void GetPlanes(ViewFrustum vf, FStaticArray<FPlane, 6>& planes);

private:
	FMatrix4 viewProjection;
	FPlane top, bottom, left, right, znear, zfar;
};

