#include "Game.h"

#include "Core/Engine/FlameRI/OpenGL/OpenGLFRI.h"
#include "Core/Engine/FlameRI/D3D11/D3D11FRI.h"
#include "Core/Engine/FlameRI/D3D12/D3D12FRI.h"


#include "Physics/PX/FPXService.h"
#include "Physics/PX/FPXAllocator.h"
#include "Physics/PX/FPXScene.h"

#include <future>



FGameTime updateGameTime;
FTimeSpan updateLastTick = FTime::GetTimestamp();

GameApplication::GameApplication(const FString& Name) :
	ApplicationName(Name),
	FriContext(nullptr),
	currentScene(NULL)
{
}

void GameApplication::CreateContext(FRIRenderingContextDescription desc)
{
	if (desc.RenderFramework == EFRIRendererFramework::OpenGL)
	{
		//FriContext = new OpenGLFRIContext(desc, NULL);
	}
	else if (desc.RenderFramework == EFRIRendererFramework::DX11)
	{
		//FriContext = new D3D11FRIContext(desc, NULL);
	}
	else if (desc.RenderFramework == EFRIRendererFramework::DX12)
	{
		FriContext = new D3D12FRIContext(desc, NULL);
	}

	FriContext->InputHandlerDelegate = FKeyEventBindingDelegate::Make<GameApplication, &GameApplication::InputHandlerFunc>(this);
	FriContext->InputHandlerDelegate2 = FMouseKeyEventBindingDelegate::Make<GameApplication, &GameApplication::MouseInputHandlerFunc>(this);
	FriContext->Initialize();

	Content.RenderContext = FriContext;

	FriContext->CreateCommandContext();
	auto CmdContext = FriContext->GetCommandContext(0);

	Renderer.CreateResources(FriContext);
	UXRenderer.LoadResources(FriContext);

}


GameApplication::~GameApplication()
{

}

void GameApplication::Frame()
{
	if (FriContext->HandleEvents())
		return;

	auto TimeStamp = FTime::GetTimestamp();
	updateGameTime.TotalTicks++;
	updateGameTime.DeltaTime = TimeStamp - updateLastTick;
	updateLastTick = TimeStamp;


	FRICommandList cmdList(FriContext->GetCommandContext(0));

	BeginRender(cmdList);
	{
		if (CurrentScene())
		{
			CurrentScene()->Update(updateGameTime);
			CurrentScene()->FinishUpdate();
			this->Update(updateGameTime);

			Renderer.Render(cmdList);


			if (CurrentScene()->uxContainer)
			{
				UXRenderer.Render(cmdList, CurrentScene()->uxContainer->GetSurface());
			}
		}
	}
	
	EndRender(cmdList);


	FriContext->SwapBuffers();
}

void GameApplication::LaunchGameThread()
{
	/*FGameTime updateGameTime;
	FTimeSpan updateLastTick = FTime::GetTimestamp();
	FTimeSpan loopLastTick = updateLastTick;

	float accumulator = 0;

	while (FriContext->IsActive())
	{
		auto loopStamp = FTime::GetTimestamp();
		float loopDelta = (loopStamp - loopLastTick).GetMilliseconds();
		loopLastTick = loopStamp;

		accumulator += loopDelta;

		if (accumulator >= timeFrame)
		{
			auto TimeStamp = FTime::GetTimestamp();
			updateGameTime.TotalTicks++;
			updateGameTime.DeltaTime = TimeStamp - updateLastTick;
			updateLastTick = TimeStamp;


			PROFILE_START("Scene Update", upTime);
			{
				currentScene->Update(updateGameTime);
			}
			PROFILE_END(upTime);

			//std::cout << "Update: " << updateGameTime.DeltaTime.GetMilliseconds() << "ms" << std::endl;

			accumulator -= timeFrame;
		}
	}*/
}

void GameApplication::LaunchRenderThread()
{
	bool firstLoop = true;

	while (FriContext->IsActive())
	{
		Frame();
		firstLoop = false;
	}
}

void GameApplication::Run()
{
	Renderer.AttachToScene(CurrentScene());
	this->LaunchRenderThread();
}

void GameApplication::InputHandlerFunc(FKeyboardKeys key, FKeyEvent keyEvent)
{
	CurrentScene()->uxContainer->HandleInput(key, keyEvent);

	CurrentScene()->System<Input>()->ForEach([&](Entity ent, Input& inputRef)
		{
			for (auto& binding : inputRef.KeyEventBindings)
			{
				binding(key, keyEvent);
			}
		});
		
}

void GameApplication::MouseInputHandlerFunc(FMouseButton key, FKeyEvent keyEvent)
{
	CurrentScene()->System<Input>()->ForEach([&](Entity ent, Input& inputRef)
		{
			for (auto& binding : inputRef.MouseEventBindings)
			{
				binding(key, keyEvent);
			}
		});

}

void GameApplication::BeginRender(FRICommandList& cmdList)
{
	cmdList.BeginFrame();
	Renderer.BeginRender(cmdList);

}

void GameApplication::EndRender(FRICommandList& cmdList)
{
	Renderer.EndRender(cmdList);
	cmdList.EndFrame();
}

bool GameApplication::IsContextActive()
{
	return FriContext->IsActive();
}

void GameApplication::TransitionToScene(Scene* scene)
{
	if (scene)
	{
		if (currentScene)
			currentScene->OnDestroyResources();
		currentScene = scene;
		scene->OnCreateResources(FriContext, Content);
		Renderer.AttachToScene(CurrentScene());
	}
	else
	{
		currentScene = NULL;
	}

}

Scene* GameApplication::CurrentScene()
{
	return currentScene;
}