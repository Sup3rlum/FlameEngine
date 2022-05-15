#pragma once

#include "Core/Common/CoreCommon.h"
#include "Core/Math/Module.h"
#include "Core/Engine/FlameRI/FRIResource.h"

struct PhysicsTriangleMeshDesc
{
	FRICreationDescriptor VertexData;
	FRICreationDescriptor IndexData;
	size_t DataStride;

	PhysicsTriangleMeshDesc(const PhysicsTriangleMeshDesc& other) :
		VertexData(other.VertexData),
		IndexData(other.IndexData),
		DataStride(other.DataStride)
	{}

	PhysicsTriangleMeshDesc() :
		DataStride(0)
	{}
};