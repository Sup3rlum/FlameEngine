#include "DirectionalLight.h"

DirectionalLight::DirectionalLight(FVector3 _direction, Color _lightColor, float _intensity)
{
	Direction = _direction;
	LightColor = _lightColor;
	Intensity = _intensity;

	//_positionInternal = FVector3(20.0f, 20.0f, 20.0f);
	_positionInternal = FVector3(100.0f, 100.0f, 100.0f);

	_cam.Projection = FMatrix4::CreateOrthographic(-200.0f, 200.0f, -200.0f, 200.0f, 0.1f, 500.0f);
	_cam.Position = _positionInternal;
	_cam.LookDirection = Direction;
	_cam.Up = FVector3(0, 1, 0);

}

void DirectionalLight::SnapToFrustum(Camera* cam)
{
	localSpace = FMatrix3::Transpose(FMatrix4::ToMatrix3(FMatrix4::CreateView(FVector3(0), Direction, _cam.Up)));
	

	FVector3 corners[8];

	cam->GetFrustumCorners(corners);

	aabb.SetDegenerate();

	for (int i = 0; i < 8; i++)
	{
		FVector3 _p = localSpace * corners[i];

		aabb.Enclose(_p);
	}


	_cam.Position = FVector3(160, 260, 0);// FMatrix3::Transpose(localSpace)* (aabb.Center() + FVector3(0, 0, aabb.LengthZ() / 2.0f));

	//_cam.Projection = FMatrix4::CreateOrthographic(-aabb.LengthX() / 2, aabb.LengthX() / 2, -aabb.LengthY() /2, aabb.LengthY() / 2, 0.1f, aabb.LengthZ());
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