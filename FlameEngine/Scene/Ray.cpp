#include "Ray.h"


Ray::Ray(fVector3 p, fVector3 d)
{
	Position = p;
	Direction = d;
}

bool Ray::Intersects(Plane p, fVector3 &out)
{
	float mu = (-p._internal.w - fVector3::Dot(Position, p.Normal())) / fVector3::Dot(Direction, p.Normal());

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