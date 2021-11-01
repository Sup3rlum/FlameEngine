#include "Game.h"

#include "Core/Engine/FlameRI/OpenGL/OpenGLFRI.h"
#include "Core/Engine/FlameRI/DX11/D3D11FRI.h"


#include "Physics/PX/FPXService.h"
#include "Physics/PX/FPXAllocator.h"
#include "Physics/PX/FPXScene.h"

GameApplication::GameApplication(const FString& Name, EFRIRendererFramework efri, Win32Context* context) :
	applicationName(Name),
	FriContext(nullptr)
{


	FRIRenderingContextDescription desc;


	if (context)
	{
		desc.Width = context->Width;
		desc.Height = context->Height;
		desc.IsFullscreen = false;
		desc.SampleCount = 0;
	}
	else
	{
		desc.Width = 2560;
		desc.Height = 1440;
		desc.IsFullscreen = false;
		desc.SampleCount = 0;
	}



	if (efri == EFRIRendererFramework::OpenGL)
	{
		FriContext = new OpenGLFRIContext(desc, context);
	}
	else if (efri == EFRIRendererFramework::DX11)
	{
		FriContext = new D3D11FRIContext(desc, NULL);
	}

	FriContext->InputHandlerDelegate = FKeyEventBindingDelegate::Make<GameApplication, &GameApplication::InputHandlerFunc>(this);
	FriContext->Initialize();
	
	Renderer.CreateResources(FriContext);
	UXRenderer.LoadResources(FriContext);

	currentScene = new Scene(CreatePhysicsSceneDescription(), FriContext);
	currentScene->LoadSystems();

}
GameApplication::~GameApplication()
{

}


void GameApplication::Update(FGameTime gameTime)
{


}

void GameApplication::Frame()
{

	auto TimeStamp = FTime::GetTimestamp();
	gameTime.TotalTicks++;
	gameTime.DeltaTime = TimeStamp - LastTickTimestamp;
	LastTickTimestamp = TimeStamp;


	FRICommandList cmd(FriContext->GetFRIDynamic());

	BeginRender(cmd);


	currentScene->Update(gameTime);
	Update(gameTime);


	Renderer.Render(cmd);
	if (currentScene->uxContainer)
	{
		UXRenderer.Render(cmd, currentScene->uxContainer->GetSurface());
	}

	EndRender(cmd);

	//cmd.Flush();

	FriContext->SwapBuffers();
	FriContext->HandleEvents();

}

void GameApplication::Run()
{
	Renderer.AttachToScene(currentScene);


	while (FriContext->IsActive())
	{
		Frame();
	}
}

void GameApplication::InputHandlerFunc(FKeyboardKeys key, FKeyboardKeyEvent keyEvent)
{
	if (key == FKeyboardKeys::Escape)
	{
		FriContext->PollCloseEvent();
	}


	/*
	ControlSystem.ForEach([&](Entity ent, ControlComponent& controlRef) 
		{
			for (int j = 0; j < controlRef.KeyEventBindings.Length(); j++)
			{
				controlRef.KeyEventBindings[j](key, keyEvent);
			}
		});
		*/
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


PhysicsSceneDescription GameApplication::CreatePhysicsSceneDescription()
{
	PhysicsSceneDescription pDesc;
	FPXService* fpxService = new FPXService();
	PhysicsScene* fpxScene = fpxService->CreateScene(FVector3(0, -10.0f, 0));

	pDesc.pService = fpxService;
	pDesc.pScene = fpxScene;
	pDesc.pAllocator = new FPXAllocator(fpxService, static_cast<FPXScene*>(fpxScene));
	
	return pDesc;
}

bool GameApplication::IsContextActive()
{
	return FriContext->IsActive();
}