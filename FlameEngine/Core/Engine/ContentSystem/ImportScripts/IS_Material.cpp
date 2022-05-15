#include "Material.h"


struct FMaterialMapHeader
{
	uint32 DimX;
	uint32 DimY;
	uint64 ByteSize;
};

Material TContentSerializer<Material>::Serialize(Stream& fileStream)
{
	FRIDynamicAllocator* allocator = renderContext->GetFRIDynamic();
	Material::MapCollection maps;

	for (int i = 0; i < 4; i++)
	{

		int a = sizeof(FMaterialMapHeader);

		FMaterialMapHeader mapHeader = fileStream.Read<FMaterialMapHeader>();
		FArray<FRIByte> mapData(mapHeader.ByteSize);
		fileStream.Read(mapData);

		auto friTex = allocator->CreateTexture2D(
			mapHeader.DimX,
			mapHeader.DimY,
			0,
			EFRITextureFormat::RGBA8UNORM,
			FRIColorDataFormat(EFRIChannels::RGBA, EFRIPixelStorage::UnsignedByte),
			FRICreationDescriptor(mapData.Begin(), mapData.ByteSize())
		);

		maps[i] = MaterialMap(friTex);
		//allocator->FlushMipMaps(friTex);
	}


	return Material(maps, renderContext);
}