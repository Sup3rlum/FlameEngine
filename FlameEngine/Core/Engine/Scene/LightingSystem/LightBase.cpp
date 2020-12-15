#include "Light.h"


Light::Light(Color _color)
{
	LightColor = _color;
}
Light::Light()
{
	LightColor = Color::White;
}
void Light::Update()
{
	_cam.Update();
}

Camera* Light::LightCamera()
{
	return &_cam;
}

void Light::DrawFlare()
{

}

void Light::CreateFlares()
{

}