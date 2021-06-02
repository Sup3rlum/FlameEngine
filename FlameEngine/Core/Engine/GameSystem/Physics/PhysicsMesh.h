#pragma once

#include "Core/Common/CoreCommon.h"
#include "Core/Math/Module.h"


struct PhysicsTriangleMeshDesc
{
	FArray<byte> VertexData;
	FArray<uint32> IndexData;
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