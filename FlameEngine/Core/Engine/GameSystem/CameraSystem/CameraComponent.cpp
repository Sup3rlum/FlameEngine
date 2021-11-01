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





void CameraComponent::GetFrustumCorners(FStaticArray<FVector3, 8>& Corners)
{
	//FMatrix4 inverseVPMatrix = FMatrix4::Transpose(FMatrix4::Inverse(View * Projection));
	FMatrix4 inverseVPMatrix = FMatrix4::Transpose(FMatrix4::Inverse(Projection * View));

	for (uint32_t i = 0; i < 8; i++)
	{
		FVector4 p = inverseVPMatrix * DXNdcCornersVector4[i];
		Corners[i] = p.xyz / p.w;
	}
}