#include "Game.h"

#include "Core/Engine/FlameRI/OpenGL/OpenGLFRI.h"


#include "Physics/PX/FPXService.h"
#include "Physics/PX/FPXAllocator.h"
#include "Physics/PX/FPXScene.h"

GameApplication::GameApplication(const FString& Name) :
	applicationName(Name),
	FriContext(nullptr)
{


	FRIRenderingContextDescription desc;

	desc.Width = 2560;
	desc.Height = 1440;
	desc.IsFullscreen = false;
	desc.SampleCount = 8;



	FriContext = new OpenGLFRIContext(desc, NULL);


	FriContext->InputHandlerDelegate = FKeyEventBindingDelegate::Make<GameApplication, &GameApplication::InputHandlerFunc>(this);
	FriContext->Initialize();
	

	renderer.CreateResources(FriContext);

	currentScene = new Scene(CreatePhysicsSceneDescription());
	currentScene->LoadSystems();

}
GameApplication::~GameApplication()
{

}


void GameApplication::Load()
{

}


void GameApplication::Suspend()
{

}

void GameApplication::Dispose()
{

}

void GameApplication::Update()
{


}

void GameApplication::Run()
{
	while (FriContext->IsActive())
	{
		
		FRICommandList cmd(FriContext->GetFRIDynamic());


		BeginRender(cmd);

		currentScene->Update();
		Update();

		EndRender(cmd);
		
		//cmd.Flush();
		
		FriContext->SwapBuffers();
		FriContext->HandleEvents();
	}
}

void GameApplication::InputHandlerFunc(FKeyboardKeys key, FKeyboardKeyEvent keyEvent)
{
	if (key == FKeyboardKeys::Escape)
	{
		FriContext->PollCloseEvent();
	}

	auto ControlSystem = currentScene->CreateSystem<ControlComponent>();

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

	renderer.BeginRender(cmdList, currentScene);
}

void GameApplication::EndRender(FRICommandList& cmdList)
{
	renderer.EndRender(cmdList);

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