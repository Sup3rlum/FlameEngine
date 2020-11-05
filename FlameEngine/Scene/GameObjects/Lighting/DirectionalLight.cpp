#include "DirectionalLight.h"

DirectionalLight::DirectionalLight(fVector3 _direction, Color _lightColor, float _intensity)
{
	Direction = _direction;
	LightColor = _lightColor;
	Intensity = _intensity;


	View = glm::lookAt(fVector3(10.0f, 10.0f, 0.0f), fVector3(10.0f, 10.0f, 0.0f) + Direction, fVector3(0, 1, 0));
	Projection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, 0.1f, 100.0f);
}