#include "PhysicsGeometry.h"
#include "PhysicsGeometryProxy.h"


BoxGeometry::BoxGeometry(FVector3 halfDim) :
	HalfDim(halfDim)
{

	PxGeometry* pxPtr = new PxBoxGeometry(halfDim.x, halfDim.y, halfDim.x);
	this->FPXGeometry = static_cast<PhysicsGeometry::FPXGeometryProxy*>(pxPtr);
}


SphereGeometry::SphereGeometry(float radius) :
	Radius(radius)
{
	PxGeometry* pxPtr = new PxSphereGeometry(radius);
	this->FPXGeometry = static_cast<PhysicsGeometry::FPXGeometryProxy*>(pxPtr);
}

CapsuleGeometry::CapsuleGeometry(float radius, float halfheight) :
	Radius(radius),
	HalfHeight(halfheight)
{
	PxGeometry* pxPtr = new PxCapsuleGeometry(radius, halfheight);
	this->FPXGeometry = static_cast<PhysicsGeometry::FPXGeometryProxy*>(pxPtr);
}


PlaneGeometry::PlaneGeometry() 
{
	PxGeometry* pxPtr = new PxPlaneGeometry();
	this->FPXGeometry = static_cast<PhysicsGeometry::FPXGeometryProxy*>(pxPtr);
}


/*
*	TriangleMesh
*/

