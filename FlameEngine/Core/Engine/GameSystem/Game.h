#pragma once

#include "Core/Common/CoreCommon.h"
#include "Common/Scene.h"
#include "Core/Engine/FlameRI/FRI.h"

#include "Core/Engine/Renderer/Deferred/DeferredRenderer.h"
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

	Scene* currentScene;

public:
	GameApplication(const FString& name);
	virtual ~GameApplication();

	virtual void Load() = 0;
	virtual void Dispose() = 0;
	virtual void Suspend() = 0;
	virtual void Update(FGameTime gameTime) = 0;
	void CreateContext(FRIRenderingContextDescription desc);
	void LaunchGameThread();
	void LaunchRenderThread();
	void Run();
	void Frame();

	void BeginRender(FRICommandList& cmdList);
	void EndRender(FRICommandList& cmdList);
	bool IsContextActive();
	void TransitionToScene(Scene* scene);
	Scene* CurrentScene();
	
	FString ApplicationName;

	DeferredRenderer Renderer;
	UXRenderer UXRenderer;
	
	FAssetManager Content;
};

