#include "PhysicsTriangleMesh.h"


/*
*	vLength
*	vertexSize
*	iLength
*	indexType
*	vdeclLength
*
*	vdecl
*	vdata
*	idata
*/


struct FMeshGeometryHeader
{
	uint64 vLength;
	uint64 elementSize;
	uint64 iLength;
};


PhysicsTriangleMeshDesc TContentSerializer<PhysicsTriangleMeshDesc>::Serialize(Stream& fileStream)
{

	uint32 vertexNumber;
	uint32 elementNumber;
	FMeshGeometryHeader geomHeader = fileStream.Read<FMeshGeometryHeader>();
	PhysicsTriangleMeshDesc TrMesh;


	TrMesh.VertexData.ByteSize = geomHeader.vLength * geomHeader.elementSize;
	TrMesh.VertexData.DataArray = Memory::Alloc<void>(TrMesh.VertexData.ByteSize);

	TrMesh.IndexData.ByteSize = geomHeader.iLength * sizeof(uint32);
	TrMesh.IndexData.DataArray = Memory::Alloc<void>(TrMesh.IndexData.ByteSize);

	TrMesh.DataStride = geomHeader.elementSize;

	fileStream.Read(TrMesh.VertexData.DataArray, TrMesh.VertexData.ByteSize);
	fileStream.Read(TrMesh.IndexData.DataArray, TrMesh.IndexData.ByteSize);

	return TrMesh;
}