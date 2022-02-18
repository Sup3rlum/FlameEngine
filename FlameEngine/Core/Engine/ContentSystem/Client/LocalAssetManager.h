#pragma once


#include "Core/Framework/IO/FileStream.h"
#include "AssetManager.h"


class FAssetManager : public IAssetManager
{
	FString Url;

public:

	FAssetManager();
	FAssetManager(const FString& path);
	FString GetURL() override;
	Status GetStatus() override;
	bool Connect(const FString& str) override;

	template<typename TComponent, typename...TArgs>
	TComponent Load(const FString& filepath, TArgs... targs)
	{		
		auto fullFilepath = FString::Format("%0/%1", GetURL(), filepath);
		IOFileStream fileStream(fullFilepath);

		FStaticAnsiString<4> fileSignature;
		FStaticArray<byte, 4> fileVersion;
		FStaticArray<byte, 16> fileChecksum;

		fileStream.ReadArray(fileSignature);
		fileStream.ReadArray(fileVersion);
		fileStream.ReadArray(fileChecksum);

		/*if (fileSignature != descriptor.Signature)
		{
			// TODO: Not matching signature
		}*/

		return TContentSerializer<TComponent>(targs...).Serialize(fileStream);

	}
};