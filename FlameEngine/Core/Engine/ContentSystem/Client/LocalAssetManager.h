#pragma once


#include "Core/Framework/IO/FileStream.h"
#include "../AssetManager.h"


EXPORT(class, FAssetManager) : public IAssetManager
{
	FString Url;

public:

	FAssetManager();
	FAssetManager(const FString& path);
	FString GetURL() override;
	Status GetStatus() override;
	bool Connect(const FString& str) override;

	template<typename TComponent, typename...TArgs>
	TComponent Load(const FString& filepath, const TArgs&... targs)
	{		
		auto fullFilepath = FString::Format("%0/%1", GetURL(), filepath);
		IOFileStream fileStream(fullFilepath);

		FStaticAnsiString<4> fileSignature;
		FStaticArray<byte, 4> fileVersion;
		FStaticArray<byte, 16> fileChecksum;

		fileStream.Read(fileSignature);
		fileStream.Read(fileVersion);
		fileStream.Read(fileChecksum);

		return TContentSerializer<TComponent>(this, targs...).Serialize(fileStream);
	}

};