#pragma once

#include "Core/Common/CoreCommon.h"
#include "Core/Math/Module.h"


enum class EPGeometryType
{
	Box,
	Sphere,
	Capsule,
	Plane,
	TriangleMesh
};


struct PhysicsGeometry
{
protected:

	class FPXGeometryProxy;
	friend class FPXAllocator;
	friend class RigidBody;
	friend class StaticRigidBody;
	friend class CharacterBody;
	FPXGeometryProxy* FPXGeometry;

public:
	virtual EPGeometryType GetGeometryType() const = 0;
	virtual ~PhysicsGeometry()
	{}
};

template<EPGeometryType TGeomType>
struct TPhysicsGeometry : PhysicsGeometry
{
	EPGeometryType GetGeometryType() const
	{
		return TGeomType;
	}
};


/* Geometry Typess */

EXPORT(struct, BoxGeometry) : TPhysicsGeometry<EPGeometryType::Box>
{
private:
	FVector3 HalfDim;
public:
	BoxGeometry(FVector3 halfDim);
};

EXPORT(struct, SphereGeometry) : TPhysicsGeometry<EPGeometryType::Sphere>
{
private:
	float Radius;
public:
	SphereGeometry(float radius);
};


EXPORT(struct, CapsuleGeometry) : TPhysicsGeometry<EPGeometryType::Capsule>
{
private:
	float Radius;
	float HalfHeight;
public:
	CapsuleGeometry(float radius, float halfheight);
};

EXPORT(struct, PlaneGeometry) : TPhysicsGeometry<EPGeometryType::Plane>
{
public:
	PlaneGeometry();
};


EXPORT(struct, TriangleMeshGeometry) : public TPhysicsGeometry<EPGeometryType::TriangleMesh>
{
public:
	//TriangleMeshGeometry(const TriangleMeshGeometry& other);

};