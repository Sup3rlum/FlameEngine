#include "Material.h"



enum class ColorChannels
{
	Red = 1,
	Green = 2,
	Blue = 4,
	Alpha = 8
};


struct FMaterialHeader
{
	uint32 MapCount;
};

struct FMaterialMapHeader
{
	EMaterialMap mapType;
	uint32 DimX;
	uint32 DimY;
	uint32 Channels;
	uint32 Format;
	uint32 Dummy;
	uint64 ByteSize;
};



MaterialComponent FMaterialSerializer::Serialize(IOFileStream& fileStream)
{
	FRIDynamicAllocator* allocator = renderContext->GetFRIDynamic();
	MaterialComponent::MapCollection maps;
	
	FMaterialHeader matHeader = fileStream.Read<FMaterialHeader>();


	FResourceTextureColorDescriptor colorDescriptor(EFRITextureChannelStorage::RGBA32F, EFRITextureChannels::RGBA, EFRITexturePixelStorage::UnsignedByte);

	for (int i = 0; i < matHeader.MapCount; i++)
	{
		FMaterialMapHeader mapHeader = fileStream.Read<FMaterialMapHeader>();
		FArray<FRIByte> mapData(mapHeader.ByteSize);
		fileStream.ReadArray<FRIByte>(mapData);

		maps[(size_t)mapHeader.mapType] = MaterialMap(allocator->DynamicCreateTexture2D(
			mapHeader.DimX,
			mapHeader.DimY,
			0,
			colorDescriptor,
			FResourceCreationDescriptor(mapData.Begin(), mapData.ByteSize())
		));
	}


	return MaterialComponent(maps, renderContext);
}