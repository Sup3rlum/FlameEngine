#pragma once

#include "Core/Common/CoreCommon.h"

#include "../PhysicsShape.h"
#include "PhysXPXCast.h"



using namespace physx;



struct FPXShape : public PhysicsShape
{
	PxShape* pxShape;


	FPXShape(PxPhysics* pPx, PhysicsMaterial mat, BoxGeometry geom) : 
		pxShape(NULL), 
		PhysicsShape(mat, EPGeometryType::Box)
	{
		MakePx(pPx, mat, PxBoxGeometry(physx_cast(geom.HalfDim)));
	}

	FPXShape(PxPhysics* pPx, PhysicsMaterial mat, SphereGeometry geom) :
		pxShape(NULL),
		PhysicsShape(mat, EPGeometryType::Sphere)
	{
		MakePx(pPx, mat, PxSphereGeometry(geom.Radius));
	}


	FPXShape(PxPhysics* pPx, PhysicsMaterial mat, PlaneGeometry geom) :
		pxShape(NULL),
		PhysicsShape(mat, EPGeometryType::Plane)
	{
		MakePx(pPx, mat, PxPlaneGeometry());
	}


	FPXShape(PxPhysics* pPx, PhysicsMaterial mat, CapsuleGeometry geom) :
		pxShape(NULL),
		PhysicsShape(mat, EPGeometryType::Plane)
	{
		MakePx(pPx, mat, PxCapsuleGeometry(geom.Radius, geom.HalfHeight));
	}




	void MakePx(PxPhysics* pPx, const PhysicsMaterial& mat, const PxGeometry& geom)
	{
		pxShape = pPx->createShape(geom, *pPx->createMaterial(mat.StaticFriction, mat.DynamicFriction, mat.Restitution));
	}

};