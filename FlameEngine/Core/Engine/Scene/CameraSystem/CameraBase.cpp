#include "Camera.h"


Camera::Camera()
{

}
Camera::~Camera()
{

}

void Camera::Update()
{
	View = FMatrix4::CreateView(Position, Position + LookDirection, Up);
	DebugView = FMatrix4::CreateView(-LookDirection * 20.0f, FVector3(0), Up);


	FMatrix4 m2(FMatrix3::Transpose(FMatrix4::ToMatrix3(View)));
	m2[3][3] = 1.0f;

	//ViewInverse = FMatrix4::Translation(Position) * m2;
	ViewInverse = FMatrix4::Inverse(View);
	ProjectionInverse = FMatrix4::Inverse(Projection);
}

void Camera::GetFrustumCorners(FVector3* corners)
{
	FVector4 ndc_corners[8] =
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



	FMatrix4 inverseVPMatrix = FMatrix4::Transpose(ViewInverse * ProjectionInverse);


	for (uint32_t i = 0; i < 8; i++)
	{
		FVector4 p = inverseVPMatrix * ndc_corners[i];

		p /= p.w;

		corners[i] = p.xyz;
	}


}