#pragma once


#include "Core/Framework/IO/FileStream.h"


enum class ELocalContentFileStorage
{
	BINARY,
	JSON,
	XML
};



struct FLocalContentFileDescriptionBase
{
	FStaticAnsiString<4> Signature;
	ELocalContentFileStorage FileStorage;

	FLocalContentFileDescriptionBase(FStaticAnsiString<4> Signature, ELocalContentFileStorage FileStorage) :
		Signature(Signature),
		FileStorage(FileStorage)
	{

	}

	FLocalContentFileDescriptionBase() = delete;
};


template<typename TComponentType>
struct TSerializerInterface
{
	virtual TComponentType Serialize(IOFileStream& fileStream) = 0;
};






template<typename TComponentType, typename TDescriptor, typename TSerializer>
struct FLocalContentLoader
{
	bool LoadAsync = false;
	bool ValidateChecksum = true;


public:
	FLocalContentLoader(bool LoadAsync, bool ValidateChecksum) : LoadAsync(LoadAsync), ValidateChecksum(ValidateChecksum)
	{
	}

	template<typename...TArgs>
	TComponentType LoadFromLocal(FString filepath, TArgs... targs)
	{
		TDescriptor descriptor;

		if (descriptor.FileStorage == ELocalContentFileStorage::BINARY)
		{

			IOFileStream fileStream(filepath);


			FStaticAnsiString<4> fileSignature;
			FStaticArray<byte, 4> fileVersion;
			FStaticArray<byte, 16> fileChecksum;

			fileStream.ReadArray(fileSignature);
			fileStream.ReadArray(fileVersion);
			fileStream.ReadArray(fileChecksum);

			if (fileSignature != descriptor.Signature)
			{
				// TODO: Not matching signature
			}

			return TSerializer(targs...).Serialize(fileStream);

		}

	}
};

