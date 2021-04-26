#pragma once

#include "Core/Common/CoreCommon.h"


struct FEngineEnvironment
{
	static FHashMap<FString, FString> Paths;

	static FString GetInstancePath();


	static void InitializePaths()
	{
		Paths
			.Set("Config", "./Configuration/")
			.Set("Content", "./Content/");
	}


};