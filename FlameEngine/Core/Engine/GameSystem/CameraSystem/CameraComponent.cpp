#include "CameraComponent.h"


FVector4 GLNdcCornersVector4[8] =
{
	FVector4(1.0f, -1.0f, -1.0f, 1.0f),	 // llb
	FVector4(-1.0f, -1.0f, -1.0f, 1.0f), // lrb
	FVector4(-1.0f, 1.0f, -1.0f, 1.0f),  // urb
	FVector4(1.0f, 1.0f, -1.0f, 1.0f),	 // ulb


	FVector4(1.0f, -1.0f, 1.0f, 1.0f),   // llf
	FVector4(-1.0f, -1.0f, 1.0f, 1.0f),  // lrf
	FVector4(-1.0f, 1.0f, 1.0f, 1.0f),   // urf
	FVector4(1.0f, 1.0f, 1.0f, 1.0f)	 // ulf

};


FVector4 DXNdcCornersVector4[8] =
{
	FVector4(1.0f,	-1.0f,	0.0f, 1.0f),	 // llb
	FVector4(-1.0f, -1.0f,	0.0f, 1.0f), // lrb
	FVector4(-1.0f, 1.0f,	0.0f, 1.0f),  // urb
	FVector4(1.0f, 1.0f,	0.0f, 1.0f),	 // ulb


	FVector4(1.0f, -1.0f,	1.0f, 1.0f),   // llf
	FVector4(-1.0f, -1.0f,	1.0f, 1.0f),  // lrf
	FVector4(-1.0f, 1.0f,	1.0f, 1.0f),   // urf
	FVector4(1.0f, 1.0f,	1.0f, 1.0f)	 // ulf

};



void Camera::GetFrustumCorners(FStaticArray<FVector3, 8>& Corners)
{
	FMatrix4 inverseVPMatrix = FMatrix4::Transpose(FMatrix4::Inverse(View * Projection));
	//FMatrix4 inverseVPMatrix = FMatrix4::Transpose(FMatrix4::Inverse(View * Projection));

	for (uint32_t i = 0; i < 8; i++)
	{
		FVector4 p = inverseVPMatrix * DXNdcCornersVector4[i];
		Corners[i] = p.xyz / p.w;
	}
}

void Camera::StageMemory(FRIMemoryMap& GPUMemory) const
{
	GPUMemory.Load(View);
	GPUMemory.Load(Projection);
	GPUMemory.Load(FMatrix4::Inverse(View));
	GPUMemory.Load(FMatrix4::Inverse(Projection));
}


float Camera::NearPlane() const
{
	float a = Projection[2][2];
	float b = Projection[3][2];

	return -b / (a + 1.0f);
}

float Camera::FarPlane() const
{
	float a = Projection[2][2];
	float b = Projection[3][2];

	return -b / (a - 1.0f);
}


FRay Camera::ScreenPointToRay(FVector2 screenPoint) const
{
	auto invProj = FMatrix4::Transpose(FMatrix4::Inverse(Projection));
	auto invView = FMatrix4::Transpose(FMatrix4::Inverse(View));

	auto dir = invProj * FVector4(screenPoint.x, screenPoint.y, 0, 1.0f);
	dir /= dir.w;

	// Rotate to match camera orientation.
	auto direction = invView * FVector4(dir.xyz, 0);
	auto eye = invView * FVector4(0, 0, 0, 1);

	// Output a ray from camera position, along this direction.
	return FRay(eye.xyz, FVector3::Normalize(direction.xyz));
}
