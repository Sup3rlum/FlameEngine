#include "Light.h"


Light::Light(Color _color)
{
	LightColor = _color;
}
Light::Light()
{
	LightColor = Color::white;
}