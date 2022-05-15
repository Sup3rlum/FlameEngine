#pragma once

#include "PhysicsMaterial.h"
#include "PhysicsGeometry.h"




struct PhysShape
{
private:

	PhysicsMaterial Material;
	PhysicsGeometry* Geometry;


public:
	PhysShape(PhysicsMaterial pmat, SphereGeometry geometry) :
		Material(pmat),
		Geometry(new SphereGeometry(geometry))
	{
	}

	PhysShape(PhysicsMaterial pmat, BoxGeometry geometry) :
		Material(pmat),
		Geometry(new BoxGeometry(geometry))
	{

	}

	PhysShape(PhysicsMaterial pmat, CapsuleGeometry geometry) :
		Material(pmat),
		Geometry(new CapsuleGeometry(geometry))
	{

	}

	PhysShape(PhysicsMaterial pmat, PlaneGeometry geometry) :
		Material(pmat),
		Geometry(new PlaneGeometry(geometry))
	{

	}
	PhysShape(PhysicsMaterial pmat, TriangleMeshGeometry geometry) :
		Material(pmat),
		Geometry(new TriangleMeshGeometry(geometry))
	{

	}

	PhysicsGeometry* GetGeometry() const
	{
		return Geometry;
	}
	const PhysicsMaterial& GetMaterial() const
	{
		return Material;
	}

	~PhysShape()
	{
		delete Geometry;
	}

};