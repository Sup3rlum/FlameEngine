#include "DirectionalLight.h"

DirectionalLight::DirectionalLight(fVector3 _direction, Color _lightColor, float _intensity)
{
	Direction = _direction;
	LightColor = _lightColor;
	Intensity = _intensity;

	_positionInternal = fVector3(20.0f, 20.0f, 20.0f);


	View = fMatrix4::CreateView(_positionInternal, _positionInternal + Direction, fVector3(0, 1, 0));
	Projection = fMatrix4::CreateOrthographic(-20.0f, 20.0f, -20.0f, 20.0f, 0.1f, 100.0f);

	_cam.Position = _positionInternal;
	_cam.View = View;
	_cam.Projection = Projection;
	_cam.Up = fVector3(0, 1, 0);

}

Camera* DirectionalLight::LightCamera()
{
	return &_cam;
}