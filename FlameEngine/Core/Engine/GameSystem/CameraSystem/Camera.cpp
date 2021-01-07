#include "Camera.h"


FVector4 NdcCornersVector4[8] =
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


FViewFrustum::FViewFrustum(FMatrix4 camMat)
{
	FMatrix4 inverseVPMatrix = FMatrix4::Transpose(FMatrix4::Inverse(camMat));

	for (uint32_t i = 0; i < 8; i++)
	{
		FVector4 p = inverseVPMatrix * NdcCornersVector4[i];
		Corners[i] = p.xyz / p.w;
	}

}


void Camera::Update()
{
	this->Transform = mEntityAttach->GetTransform();

	View = FViewMatrix(Transform.Position, Transform.Orientation);


	ViewInverse = FMatrix4::Inverse(View);
	ProjectionInverse = FMatrix4::Inverse(Projection);
}

FViewFrustum Camera::GetFrustum() const
{
	return FViewFrustum(View * Projection);
}
