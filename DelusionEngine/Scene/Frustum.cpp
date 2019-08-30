#include "Frustum.h"

Frustum::Frustum(mat4 PV)
{
	vec4 row[4];

	for (int i = 0; i < 4; i++)
	{
		row[i] = transpose(PV)[i];
	}

	/*zNear =		Plane(row[3] + row[2]);
	zFar =		Plane(row[3] - row[2]);
	Left =		Plane(row[3] + row[0]);
	Right =		Plane(row[3] - row[0]);
	Top =		Plane(row[3] - row[1]);
	Bottom =	Plane(row[3] + row[1]);*/

	planes[0]	= Plane(row[3] + row[2]);
	planes[1]	= Plane(row[3] - row[2]);
	planes[2]	= Plane(row[3] + row[0]);
	planes[3]	= Plane(row[3] - row[0]);
	planes[4]	= Plane(row[3] - row[1]);
	planes[5]	= Plane(row[3] + row[1]);
}

bool Frustum::Contains(BoundingBox _b)
{
	return Intersects(_b) != IntersectionType::Disjoint;
}
bool Frustum::Contains(vec3 _p)
{
	return	true;/*Left.DistanceSigned(_p) > 0 &&
			Right.DistanceSigned(_p) > 0 && 
			zNear.DistanceSigned(_p) > 0 && 
			zFar.DistanceSigned(_p) > 0;*/
}

IntersectionType Frustum::Intersects(BoundingBox _b)
{
	vec3* corns = _b.GetCorners();

	IntersectionType type = IntersectionType::Contains;

	int out;
	int in;

	for (int i = 0; i < 6; i++)
	{

		// reset counters for corners in and out
		out = 0; in = 0;
		// for each corner of the box do ...
		// get out of the cycle as soon as a box as corners
		// both inside and out of the frustum
		for (int k = 0; k < 8 && (in == 0 || out == 0); k++) 
		{

			// is the corner outside or inside
			if (planes[i].DistanceSigned(corns[k]) < 0)
				out++;
			else
				in++;
		}
		//if all corners are out
		if (!in)
			return IntersectionType::Disjoint;
		// if some corners are out and others are in
		else if (out)
			type = IntersectionType::Intersects;

	}

	return type;
}
