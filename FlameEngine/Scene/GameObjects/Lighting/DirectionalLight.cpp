#include "DirectionalLight.h"

DirectionalLight::DirectionalLight(fVector3 _direction, Color _lightColor, float _intensity)
{
	Direction = _direction;
	LightColor = _lightColor;
	Intensity = _intensity;

	//_positionInternal = fVector3(20.0f, 20.0f, 20.0f);
	_positionInternal = fVector3(100.0f, 100.0f, 100.0f);

	_cam.Projection = fMatrix4::CreateOrthographic(-200.0f, 200.0f, -200.0f, 200.0f, 0.1f, 500.0f);
	_cam.Position = _positionInternal;
	_cam.Up = fVector3(0, 1, 0);

}

void DirectionalLight::SnapToFrustum(Camera* cam)
{

	float length = 50;

	fVector3 midPoint = cam->Position + cam->LookDirection * length;

	fVector3 pos = midPoint - fVector3::Normalize(Direction) * length;


	_positionInternal = pos;
	boundingIndex = length;
}

void DirectionalLight::Update()
{
	_cam.LookDirection = Direction;
	_cam.Position = _positionInternal;

	//_cam.Projection = fMatrix4::CreateOrthographic(-boundingIndex, boundingIndex, -boundingIndex, boundingIndex, 0.1f, 3 * boundingIndex);

	__super::Update();
}
