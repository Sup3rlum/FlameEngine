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



Material TContentSerializer<Material>::Serialize(IOFileStream& fileStream)
{
	FRIDynamicAllocator* allocator = renderContext->GetFRIDynamic();
	Material::MapCollection maps;
	
	FMaterialHeader matHeader = fileStream.Read<FMaterialHeader>();

	for (int i = 0; i < matHeader.MapCount; i++)
	{
		FMaterialMapHeader mapHeader = fileStream.Read<FMaterialMapHeader>();
		FArray<FRIByte> mapData(mapHeader.ByteSize);
		fileStream.ReadArray<FRIByte>(mapData);


		EFRIChannels channels = EFRIChannels::RGB;
		switch (mapHeader.Channels)
		{
		case 1: channels = EFRIChannels::R; break;
		case 2: channels = EFRIChannels::RG; break;
		case 3: channels = EFRIChannels::RGB; break;
		case 4: channels = EFRIChannels::RGBA; break;
		default:
			break;
		}

		maps[(size_t)mapHeader.mapType] = MaterialMap(allocator->CreateTexture2D(
			mapHeader.DimX,
			mapHeader.DimY,
			0,
			EFRITextureFormat::RGBA8UNORM,
			FRIColorDataFormat(channels, EFRIPixelStorage::UnsignedByte),
			FRICreationDescriptor(mapData.Begin(), mapData.ByteSize())
		));
	}


	return Material(maps, renderContext);
}