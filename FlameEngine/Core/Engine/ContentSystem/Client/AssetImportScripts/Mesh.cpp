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
};

FMeshSerializer::FMeshSerializer(FRIContext* context) 
	: renderContext(context) 
{
	FRICommandList cmdList(context->GetFRIDynamic(), true);

	FRIVertexShader* signatureShader = NULL;
	if (renderContext->RenderFramework == EFRIRendererFramework::DX11)
	{
		signatureShader = cmdList.GetDynamic()->CreateVertexShader(IOFileStream("shaders/signature/dx/bin/Mesh.signature.cso").ReadBytes());
	}

	FArray<FRIVertexDeclarationComponent> DeclCompArray;
	DeclCompArray.Add(FRIVertexDeclarationComponent("POSITION", 3, EFRIVertexDeclerationAttributeType::Float, EFRIBool::False, 56, 0));
	DeclCompArray.Add(FRIVertexDeclarationComponent("NORMAL", 3, EFRIVertexDeclerationAttributeType::Float, EFRIBool::False, 56, 12));
	DeclCompArray.Add(FRIVertexDeclarationComponent("TANGENT", 3, EFRIVertexDeclerationAttributeType::Float, EFRIBool::False, 56, 24));
	DeclCompArray.Add(FRIVertexDeclarationComponent("BITANGENT", 3, EFRIVertexDeclerationAttributeType::Float, EFRIBool::False, 56, 36));
	DeclCompArray.Add(FRIVertexDeclarationComponent("TEXCOORD", 2, EFRIVertexDeclerationAttributeType::Float, EFRIBool::False, 56, 48));

	vertexDeclaration = cmdList.GetDynamic()->CreateVertexDeclaration(DeclCompArray, signatureShader);

	delete signatureShader;
}

MeshComponent FMeshSerializer::Serialize(IOFileStream& fileStream)
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


	/*printf("Mesh VData:\n");

	for (int i = 0; i < geomHeader.vLength; i++)
	{
		float* arr = reinterpret_cast<float*>(&vData[i*geomHeader.elementSize]);

		printf("texCoord: %f, %f\n", arr[12], arr[13]);
	}*/

	return MeshComponent(vBuffer, iBuffer);
}