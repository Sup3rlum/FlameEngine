#include "Ray.h"


Ray::Ray(Vector3 p, Vector3 d)
{
	Position = p;
	Direction = d;
}

Vector3 Ray::Intersects(Plane p)
{
	if (dot(p._internal,Vector4(Direction, 0)) ==  
}