#include "Mesh.h"


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
	EFRIIndexType indexType;
	uint32 vdeclLength;
};

FORCEINLINE size_t SwitchIndexType(EFRIIndexType type)
{
	if (type == EFRIIndexType::UInt8) return sizeof(byte);
	else if (type == EFRIIndexType::UInt16) return sizeof(uint16_t);
	else if (type == EFRIIndexType::UInt32) return sizeof(uint32);
	else return 0;
}


MeshComponent FMeshSerializer::Serialize(IOFileStream& fileStream)
{

	FRIDynamicAllocator* allocator = renderContext->GetFRIDynamic();
	FResourceVertexBuffer* vBuffer;
	FResourceIndexBuffer* iBuffer;
	uint32 vertexNumber;
	uint32 elementNumber;
	FArray<byte> vData;
	FArray<byte> iData;
	FMeshGeometryHeader geomHeader = fileStream.Read<FMeshGeometryHeader>();
	FArray<FResourceVertexDeclarationComponent> vDecl(geomHeader.vdeclLength);

	fileStream.ReadArray<FResourceVertexDeclarationComponent>(vDecl);

	vData.Resize(geomHeader.vLength * geomHeader.elementSize);
	iData.Resize(geomHeader.iLength * SwitchIndexType(geomHeader.indexType));


	fileStream.ReadArray<byte>(vData);
	fileStream.ReadArray<byte>(iData);

	vBuffer = allocator->DynamicCreateVertexBuffer(geomHeader.vLength, 0, FResourceCreationDescriptor((FResourceArrayInterface*)vData.Begin(), vData.ByteSize()));
	iBuffer = allocator->DynamicCreateIndexBuffer(geomHeader.iLength, 0, FResourceCreationDescriptor((FResourceArrayInterface*)iData.Begin(), iData.ByteSize()));

	allocator->AttachVertexDeclaration(vDecl);

	return MeshComponent(vBuffer, iBuffer, vDecl);
}