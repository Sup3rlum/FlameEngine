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
struct TContentSerializer;


class IAssetManager
{
public:

	enum class Status
	{
		Disconnected = 0,
		Connected = 1
	};

	virtual FString GetURL() = 0;
	virtual Status GetStatus() = 0;
	virtual bool Connect(const FString& url) = 0;
	virtual ~IAssetManager() {}

};

