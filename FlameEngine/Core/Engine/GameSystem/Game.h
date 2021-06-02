#pragma once

#include "Core/Common/CoreCommon.h"
#include "Common/Scene.h"
#include "Core/Engine/FlameRI/FRI.h"

#include "Core/Engine/Renderer/DeferredRenderer.h"


class Win32Context;

EXPORT(class,  GameApplication)
{
protected:
	FRIContext* FriContext;

	void InputHandlerFunc(FKeyboardKeys key, FKeyboardKeyEvent keyEvent);

public:
	GameApplication(const FString& name, EFRIRendererFramework framework, Win32Context* winContext);

	virtual void Load();
	virtual void Dispose();
	virtual void Suspend();

	virtual void Update();

	virtual void Run();


	void Frame();


	PhysicsSceneDescription CreatePhysicsSceneDescription();


	void BeginRender(FRICommandList& cmdList);
	void EndRender(FRICommandList& cmdList);

	virtual ~GameApplication();

	bool IsContextActive();

	Scene* currentScene;
	FString applicationName;

	DeferredRenderer renderer;

};

