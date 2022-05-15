#pragma once

#include "Core/Common/CoreCommon.h"
#include "Common/Scene.h"
#include "Core/Engine/FlameRI/FRI.h"

#include "Core/Engine/Renderer/DeferredRenderer.h"
#include "Core/UX/UXRenderer.h"
#include "Core/UX/UXContainer.h"
#include "Core/Engine/ContentSystem/Client/LocalAssetManager.h"

class Win32Context;


EXPORT(class,  GameApplication)
{
protected:

	FRIContext* FriContext;

	void InputHandlerFunc(FKeyboardKeys key, FKeyEvent keyEvent);
	void MouseInputHandlerFunc(FMouseButton key, FKeyEvent keyEvent);

public:
	GameApplication(const FString& name);

	virtual void Load() = 0;
	virtual void Dispose() = 0;
	virtual void Suspend() = 0;

	virtual void Update(FGameTime gameTime) = 0;

	void CreateContext(FRIRenderingContextDescription desc);


	void LaunchGameThread();
	void LaunchRenderThread();

	void Run();
	void Frame();

	PhysicsSceneDescription CreatePhysicsSceneDescription();

	void BeginRender(FRICommandList& cmdList);
	void EndRender(FRICommandList& cmdList);

	virtual ~GameApplication();

	bool IsContextActive();

	Scene* currentScene;
	FString applicationName;

	DeferredRenderer Renderer;
	UXRenderer UXRenderer;
	
	FAssetManager Content;

	FTimeSpan uxTime, swapTime, msgTime, totalTime, endTime, upTime;
};

