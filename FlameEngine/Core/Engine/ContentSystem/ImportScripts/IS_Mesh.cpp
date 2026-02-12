#include "Mesh.h"
#include "Core/Engine/Renderer/Common/Geometry/VertexComponent.h"
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

TContentSerializer<Mesh>::TContentSerializer(IAssetManager* manager)
	: renderContext(manager->RenderContext) 
{
	/*FRICommandList cmdList(renderContext->GetFRIDynamic(), true);

	FRIVertexShader* signatureShader = NULL;
	if (renderContext->InstanceDescription.RenderFramework == EFRIRendererFramework::DX11)
	{
		signatureShader = cmdList.GetDynamic()->CreateVertexShader(IOFileStream("Assets/Shaders/signature/dx/bin/Mesh.signature.cso").ReadBytes());
	}

	vertexDeclaration = cmdList.GetDynamic()->CreateInputLayout(
		{ FRIInputDesc(InputLayouts::StaticLit, 0) },
		signatureShader);

	delete signatureShader;*/
}

Mesh TContentSerializer<Mesh>::Serialize(Stream& fileStream)
{

	FRIDynamicAllocator* allocator = renderContext->GetFRIDynamic();
	FRIVertexBuffer* vBuffer;
	FRIIndexBuffer* iBuffer;
	uint32 vertexNumber;
	uint32 elementNumber;
	FArray<byte> vData;
	FArray<byte> iData;
	FMeshGeometryHeader geomHeader = fileStream.Read<FMeshGeometryHeader>();

	FRICommandList cmdList(renderContext->GetCommandContext(0));
	cmdList.Open();


	vData.Resize(geomHeader.vLength * geomHeader.elementSize);
	iData.Resize(geomHeader.iLength * sizeof(uint32));

	fileStream.Read(vData);
	fileStream.Read(iData);

	vBuffer = allocator->CreateVertexBuffer(geomHeader.vLength, geomHeader.elementSize, EFRIAccess::None, EFRIUsage::Default, FRICreationDescriptor(vData.Begin(), vData.ByteSize()));
	iBuffer = allocator->CreateIndexBuffer(geomHeader.iLength, EFRIAccess::None, EFRIUsage::Default, FRICreationDescriptor(iData.Begin(), iData.ByteSize()));

	cmdList.CloseAndExecute();
	return Mesh(vBuffer, iBuffer);
}