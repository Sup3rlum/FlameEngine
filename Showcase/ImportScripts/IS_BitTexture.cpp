#include "BitTexture.h"


struct FMaterialMapHeader
{
	uint32 DimX;
	uint32 DimY;
	uint64 ByteSize;
};

BitTexture TContentSerializer<BitTexture>::Serialize(Stream& fileStream)
{

	FMaterialMapHeader mapHeader = fileStream.Read<FMaterialMapHeader>();
	FRICreationDescriptor descriptor;

	descriptor.ByteSize = mapHeader.ByteSize;
	descriptor.DataArray = Memory::Alloc<byte>(mapHeader.ByteSize);

	fileStream.Read(descriptor.DataArray, descriptor.ByteSize);

	return BitTexture(descriptor);
}