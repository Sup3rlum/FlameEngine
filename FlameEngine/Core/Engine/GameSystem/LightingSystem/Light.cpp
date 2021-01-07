#include "DirectionalLight.h"




DirectionalLight::DirectionalLight(const FVector3& direction, Color lightColor, float intensity) :
	Direction(direction),
	localSpace(FMatrix3(1))

{
	LightColor = lightColor;
	Intensity = intensity;

}

void DirectionalLight::SnapToFrustum(Camera* cam)
{
	localSpace = FMatrix3::Transpose(FMatrix4::ToMatrix3(FViewMatrix(FVector3(0), Direction, _cam.Up)));
	

	FVector3 corners[8];

	cam->GetFrustumCorners(corners);

	aabb.SetDegenerate();

	for (int i = 0; i < 8; i++)
	{
		FVector3 _p = localSpace * corners[i];

		aabb.Enclose(_p);
	}


	_cam.Position = FMatrix3::Transpose(localSpace)* (aabb.Center() + FVector3(0, 0, aabb.LengthZ() / 2.0f));

	_cam.Projection = FOrthographicMatrix(-aabb.LengthX() / 2, aabb.LengthX() / 2, -aabb.LengthY() /2, aabb.LengthY() / 2, 0.1f, aabb.LengthZ());
}

void DirectionalLight::Update()
{
	__super::Update();
}

void DirectionalLight::CreateFlare()
{

}
void DirectionalLight::DrawFlare()
{

}