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


PhysicsTriangleMeshDesc TContentSerializer<PhysicsTriangleMeshDesc>::Serialize(IOFileStream& fileStream)
{

	uint32 vertexNumber;
	uint32 elementNumber;
	FMeshGeometryHeader geomHeader = fileStream.Read<FMeshGeometryHeader>();
	PhysicsTriangleMeshDesc TrMesh;

	TrMesh.VertexData.Resize(geomHeader.vLength * geomHeader.elementSize);
	TrMesh.IndexData.Resize(geomHeader.iLength);
	TrMesh.DataStride = geomHeader.elementSize;

	fileStream.ReadArray<byte>(TrMesh.VertexData);
	fileStream.ReadArray<uint32>(TrMesh.IndexData);

	return TrMesh;
}