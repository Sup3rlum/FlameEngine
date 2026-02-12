#include "Material.h"


struct FMaterialMapHeader
{
	uint32 DimX;
	uint32 DimY;
	uint64 ByteSize;
};


const FString LayerNames[4] = {
	"Albedo",
	"Normal",
	"Detail"
	"Emissive"
};

Material TContentSerializer<Material>::Serialize(Stream& fileStream)
{
	FRIDynamicAllocator* allocator = renderContext->GetFRIDynamic();
	FStaticArray<MaterialLayer, 4> layers;

	FRICommandList cmdList(renderContext->GetCommandContext(0));
	cmdList.Open();
	
	for (int i = 0; i < 4; i++)
	{

		int a = sizeof(FMaterialMapHeader);

		FMaterialMapHeader mapHeader = fileStream.Read<FMaterialMapHeader>();
		FArray<FRIByte> mapData(mapHeader.ByteSize);
		fileStream.Read(mapData);

		auto friTex = allocator->CreateTexture2D(
			mapHeader.DimX,
			mapHeader.DimY,
			1,
			EFRIAccess::None,
			EFRITextureFormat::RGBA8UNORM,
			new FRICreationDescriptor(mapData.Begin(), mapData.ByteSize())
		);

		layers[i].Handle = friTex;
		layers[i].View = allocator->CreateShaderResourceView(friTex);
		layers[i].Sampler = allocator->CreateSamplerState(EFRITextureFilter::Anisotropic8, EFRITextureAddress::Repeat, EFRITextureAddress::Repeat, EFRITextureAddress::Repeat);

		//friTex->Rename(LayerNames[i]);

		//allocator->FlushMipMaps(friTex->View());
	}

	cmdList.CloseAndExecute();
	return Material(layers);
}