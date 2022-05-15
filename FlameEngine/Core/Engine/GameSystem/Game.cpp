#include "Game.h"

#include "Core/Engine/FlameRI/OpenGL/OpenGLFRI.h"
#include "Core/Engine/FlameRI/DX11/D3D11FRI.h"


#include "Physics/PX/FPXService.h"
#include "Physics/PX/FPXAllocator.h"
#include "Physics/PX/FPXScene.h"

#include <future>


GameApplication::GameApplication(const FString& Name) :
	applicationName(Name),
	FriContext(nullptr)
{
}

void GameApplication::CreateContext(FRIRenderingContextDescription desc)
{
	if (desc.RenderFramework == EFRIRendererFramework::OpenGL)
	{
		FriContext = new OpenGLFRIContext(desc, NULL);
	}
	else if (desc.RenderFramework == EFRIRendererFramework::DX11)
	{
		FriContext = new D3D11FRIContext(desc, NULL);
	}

	FriContext->InputHandlerDelegate = FKeyEventBindingDelegate::Make<GameApplication, &GameApplication::InputHandlerFunc>(this);
	FriContext->InputHandlerDelegate2 = FMouseKeyEventBindingDelegate::Make<GameApplication, &GameApplication::MouseInputHandlerFunc>(this);
	FriContext->Initialize();

	Content.RenderContext = FriContext;

	Renderer.CreateResources(FriContext);
	UXRenderer.LoadResources(FriContext);


	currentScene = new Scene(CreatePhysicsSceneDescription(), FriContext);
	currentScene->LoadSystems();
}



#define AVERAGE(a) a.GetSeconds() * 1000.0f / (float)updateGameTime.TotalTicks
#define PROFILE_START(name, a) auto now_##a = FTime::GetTimestamp()
#define PROFILE_END(a) a += FTime::GetTimestamp() - now_##a


FGameTime updateGameTime;
FTimeSpan updateLastTick = FTime::GetTimestamp();

GameApplication::~GameApplication()
{
	delete currentScene;

	printf("\nPhysics time:         %fms\n", AVERAGE(currentScene->physTime));
	printf("Dynamic time:         %fms\n", AVERAGE(currentScene->dynTime));
	printf("Behaviour time:         %fms\n", AVERAGE(currentScene->behTime));
	printf("Update time:         %fms\n", AVERAGE(upTime));
	printf("------------------------------------\n");
	printf("Stage time:         %fms\n", AVERAGE(Renderer.stageTime));
	printf("GI time:            %fms\n", AVERAGE(Renderer.giTime));
	printf("SM time:            %fms\n", AVERAGE(Renderer.smTime));
	printf("GBuffer time:       %fms\n", AVERAGE(Renderer.gTime));
	printf("Test time:          %fms\n", AVERAGE(Renderer.testTime));
	printf("Lighting time:      %fms\n", AVERAGE(Renderer.lightTime));
	printf("PostProcess time:   %fms\n", AVERAGE(Renderer.ppTime));
	printf("UXtime:             %fms\n", AVERAGE(uxTime));
	printf("End time:           %fms\n", AVERAGE(endTime));
	printf("Swap	time:       %fms\n", AVERAGE(swapTime));
	printf("Message	time:       %fms\n", AVERAGE(msgTime));
	printf("------------------------------------\n");
	printf("total	time:       %fms\n", AVERAGE(totalTime));
}

void GameApplication::Frame()
{


	auto TimeStamp = FTime::GetTimestamp();
	updateGameTime.TotalTicks++;
	updateGameTime.DeltaTime = TimeStamp - updateLastTick;
	updateLastTick = TimeStamp;

	PROFILE_START("Total", totalTime);

	/*auto renderTask = std::async(std::launch::async, [&]()
		{*/
		//if (firstLoop) continue;

	FRICommandList cmd(FriContext->GetFRIDynamic(), true);
	BeginRender(cmd);
	{
		PROFILE_START("Scene Update", upTime);

		PROFILE_END(upTime);

		currentScene->Update(updateGameTime);
		currentScene->FinishUpdate();
		this->Update(updateGameTime);

		Renderer.Render(cmd);

		PROFILE_START("UX", uxTime);

		if (currentScene->uxContainer)
		{
			UXRenderer.Render(cmd, currentScene->uxContainer->GetSurface());
		}
		PROFILE_END(uxTime);
	}

	PROFILE_START("End", endTime);
	EndRender(cmd);
	PROFILE_END(endTime);

	//cmd.Flush();

	PROFILE_START("SwapBuffers", swapTime);
	FriContext->SwapBuffers();
	PROFILE_END(swapTime);

	PROFILE_START("Msg Time", msgTime);
	FriContext->HandleEvents();
	PROFILE_END(msgTime);
	//});
	PROFILE_END(totalTime);

	//renderTask.wait();
}

float timeFrame = 1000.0f / 128.0f;

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
	//FGameTime updateGameTime;
	//FTimeSpan updateLastTick = FTime::GetTimestamp();

	bool firstLoop = true;

	while (FriContext->IsActive())
	{
		Frame();
		//std::cout << "Render: " << updateGameTime.DeltaTime.GetSeconds() * 1000.0f << "ms" << std::endl;

		if (firstLoop) firstLoop = false;
	}
}

void GameApplication::Run()
{
	Renderer.AttachToScene(currentScene);

	//renderThread = std::thread([&] {
		this->LaunchRenderThread();
		//});

	//LaunchGameThread();

	//renderThread.join();
}

void GameApplication::InputHandlerFunc(FKeyboardKeys key, FKeyEvent keyEvent)
{
	currentScene->uxContainer->HandleInput(key, keyEvent);

	currentScene->System<Input>()->ForEach([&](Entity ent, Input& inputRef)
		{
			for (auto& binding : inputRef.KeyEventBindings)
			{
				binding(key, keyEvent);
			}
		});
		
}

void GameApplication::MouseInputHandlerFunc(FMouseButton key, FKeyEvent keyEvent)
{
	currentScene->System<Input>()->ForEach([&](Entity ent, Input& inputRef)
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