#include "LocalAssetManager.h"

#include <filesystem>


FString FAssetManager::GetURL()
{
	return this->Url;
}
IAssetManager::Status FAssetManager::GetStatus()
{
	return Status::Connected;
}
bool FAssetManager::Connect(const FString& str)
{
	this->Url = str;
	return std::filesystem::exists(GetURL().ToPlatformString());
}


FAssetManager::FAssetManager(const FString& str) :
	Url(str)
{
}

FAssetManager::FAssetManager() :
	Url("./")
{
}