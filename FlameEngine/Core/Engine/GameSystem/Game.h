#pragma once

#include "Core/Common/CoreCommon.h"
#include "Common/Scene.h"


EXPORT(class,  GameApplication)
{

public:
	GameApplication(const FString& name);

	virtual void Load() = 0;
	virtual void Dispose() = 0;
	virtual void Suspend() = 0;


	virtual void Run();

	virtual ~GameApplication();

	FUniquePointer<Scene> currentScene;
	FString applicationName;

};

