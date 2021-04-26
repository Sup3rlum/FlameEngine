#pragma once

#include "PhysicsMaterial.h"
#include "PhysicsGeometry.h"




struct PhysicsShape
{
	PhysicsMaterial PhysMaterial;
	EPGeometryType GeometryType;

protected:
	PhysicsShape(PhysicsMaterial pmat, EPGeometryType geom) :
		PhysMaterial(pmat),
		GeometryType(geom)
	{

	}
};