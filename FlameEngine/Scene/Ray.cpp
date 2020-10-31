#include "Ray.h"


Ray::Ray(Vector3 p, Vector3 d)
{
	Position = p;
	Direction = d;
}

bool Ray::Intersects(Plane p, Vector3 &out)
{
	float mu = (-p._internal.w - dot(Position, p.Normal())) / dot(Direction, p.Normal());

	if (mu < 0)
	{
		return false;
	}
	else
	{
		out = Position + mu * Direction;
		return true;
	}
}