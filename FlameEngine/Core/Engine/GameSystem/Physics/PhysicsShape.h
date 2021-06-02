#pragma once

#include "PhysicsMaterial.h"
#include "PhysicsGeometry.h"




struct PhysicsShape
{
private:

	PhysicsMaterial Material;
	PhysicsGeometry* Geometry;



public:
	PhysicsShape(PhysicsMaterial pmat, SphereGeometry geometry) :
		Material(pmat),
		Geometry(new SphereGeometry(geometry))
	{
	}

	PhysicsShape(PhysicsMaterial pmat, BoxGeometry geometry) :
		Material(pmat),
		Geometry(new BoxGeometry(geometry))
	{

	}

	PhysicsShape(PhysicsMaterial pmat, CapsuleGeometry geometry) :
		Material(pmat),
		Geometry(new CapsuleGeometry(geometry))
	{

	}

	PhysicsShape(PhysicsMaterial pmat, PlaneGeometry geometry) :
		Material(pmat),
		Geometry(new PlaneGeometry(geometry))
	{

	}
	PhysicsShape(PhysicsMaterial pmat, TriangleMeshGeometry geometry) :
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

	~PhysicsShape()
	{
		delete Geometry;
	}

};