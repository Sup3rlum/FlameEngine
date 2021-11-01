#include "InstanceMesh.h"


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

TContentSerializer<InstanceMesh>::TContentSerializer(
	FRIContext* context, 
	const FArray<FRIVertexDeclarationComponent>& vertexDataLayout, 
	const FArray<FRIVertexDeclarationComponent>& instanceDataLayout, 
	const ShaderLibraryModulePart& signature
)
	: renderContext(context)
{
	FRICommandList cmdList(context->GetFRIDynamic(), true);


	FRIVertexShader* signatureShader = cmdList.GetDynamic()->CreateVertexShader(signature.Memory);

	FArray<FRIVertexDeclarationDesc> LayoutDesc;
	LayoutDesc.Add(FRIVertexDeclarationDesc(vertexDataLayout, 0));
	LayoutDesc.Add(FRIVertexDeclarationDesc(instanceDataLayout, 1));

	vertexDeclaration = cmdList.GetDynamic()->CreateVertexDeclaration(LayoutDesc, signatureShader);
}

InstanceMesh TContentSerializer<InstanceMesh>::Serialize(IOFileStream& fileStream)
{

	FRIDynamicAllocator* allocator = renderContext->GetFRIDynamic();
	FRIVertexBuffer* vBuffer;
	FRIIndexBuffer* iBuffer;
	uint32 vertexNumber;
	uint32 elementNumber;
	FArray<byte> vData;
	FArray<byte> iData;
	FMeshGeometryHeader geomHeader = fileStream.Read<FMeshGeometryHeader>();

	vData.Resize(geomHeader.vLength * geomHeader.elementSize);
	iData.Resize(geomHeader.iLength * sizeof(uint32));

	fileStream.ReadArray<byte>(vData);
	fileStream.ReadArray<byte>(iData);

	vBuffer = allocator->CreateVertexBuffer(geomHeader.vLength, 0, FRICreationDescriptor((FRIArrayInterface*)vData.Begin(), vData.ByteSize()));
	iBuffer = allocator->CreateIndexBuffer(geomHeader.iLength, 0, FRICreationDescriptor((FRIArrayInterface*)iData.Begin(), iData.ByteSize()));

	allocator->AttachVertexDeclaration(vBuffer, vertexDeclaration);

	return InstanceMesh(vBuffer, iBuffer);
}