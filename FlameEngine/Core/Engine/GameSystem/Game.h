#pragma once

#include "Core/Common/CoreCommon.h"
#include "Common/Scene.h"
#include "Core/Engine/FlameRI/FRI.h"


EXPORT(class,  GameApplication)
{
private:
	FRIContext* FriContext;

	void InputHandlerFunc(FKeyboardKeys key, FKeyboardKeyEvent keyEvent);

public:
	GameApplication(const FString& name);

	virtual void Load();
	virtual void Dispose();
	virtual void Suspend();


	virtual void Run();

	virtual ~GameApplication();

	FUniquePointer<Scene> currentScene;
	FString applicationName;



};

