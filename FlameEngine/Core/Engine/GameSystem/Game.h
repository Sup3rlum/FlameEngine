#pragma once

#include "Core/Common/CoreCommon.h"
#include "Common/Scene.h"
#include "Core/Engine/FlameRI/FRI.h"

#include "Core/Engine/Renderer/DeferredRenderer.h"


EXPORT(class,  GameApplication)
{
protected:
	FRIContext* FriContext;

	void InputHandlerFunc(FKeyboardKeys key, FKeyboardKeyEvent keyEvent);

public:
	GameApplication(const FString& name);

	virtual void Load();
	virtual void Dispose();
	virtual void Suspend();

	virtual void Update();

	virtual void Run();

	PhysicsSceneDescription CreatePhysicsSceneDescription();


	void BeginRender(FRICommandList& cmdList);
	void EndRender(FRICommandList& cmdList);

	virtual ~GameApplication();

	Scene* currentScene;
	FString applicationName;

	DeferredRenderer renderer;

};

