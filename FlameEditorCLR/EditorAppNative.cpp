#include "EditorAppNative.h"

#include <Platform/Windows/Win32Context.h>
#include <Core/Engine/GameSystem/Game.h>
#include <Core/Engine/GameSystem/Mesh.h>

#include "Core/Engine/ContentSystem/ImportScripts/Mesh.h"
#include "Core/Engine/ContentSystem/ImportScripts/Material.h"
#include "Core/Engine/ContentSystem/ImportScripts/EnvironmentMap.h"

#include "Core/Engine/ContentSystem/ImportScripts/Common.h"


#include "EditorViewerScript.h"

namespace FlameEditorCLR
{
	class EditorEngineApp : public GameApplication
	{

		Entity sampleSphere;

	public:
		EditorEngineApp() :
			GameApplication("FlameEditor")
		{

		}

		void Load() override
		{
			auto PerspMatrix = FPerspectiveMatrix(PI / 3, FriContext->GetViewport().AspectRatio(), 0.1f, 300.0f);

			Entity cameraEnt = CurrentScene->CreateEntity<Camera, FPComponent, Input, Behaviour>("camera");

			cameraEnt.Component<FTransform>().Position = FVector3(5, 1, 5);
			cameraEnt.Component<FTransform>().Orientation = FQuaternion::FromEulerAngles(FVector3(0, PI, 0));
			cameraEnt.Component<Camera>() = Camera(PerspMatrix);
			cameraEnt.Component<Behaviour>().AttachNew<EditorViewerScript>(FriContext, this);

			CurrentScene->Elements["GameCamera"] = cameraEnt;

			auto levelEnt = CurrentScene->CreateEntity<Level>("Map");
			levelEnt.Component<Level>() = Content.Load<Level>("Maps/test.flen", CurrentScene);

			CurrentScene->Elements["Sun"] = CurrentScene->CreateEntity<DirectionalLight>("Sun");
			CurrentScene->Elements["Sun"].Component<DirectionalLight>().Direction = FVector3::Normalize(FVector3(0, -1.0f, -1.0f));
			CurrentScene->Elements["Sun"].Component<DirectionalLight>().Color = Color::White;
			CurrentScene->Elements["Sun"].Component<DirectionalLight>().Intensity = 5.0f;


			sampleSphere = CurrentScene->CreateEntity<Model>("sampleSphere");
			sampleSphere.Component<Model>().Mesh = Content.Load<Mesh>("Models/uvSphere.fl3d");
			sampleSphere.Component<Model>().Material = Content.Load<Material>("Materials/gold.flmt");
			sampleSphere.Component<FTransform>() = FTransform();


			CurrentScene->Elements["Environment"] = CurrentScene->CreateEntity<EnvironmentMap>("EnvMap");
			CurrentScene->Elements["Environment"].Component<EnvironmentMap>() = Content.Load<EnvironmentMap>("Materials/forest_hdr.flenv");

		}

		void Update(FGameTime gameTime) override
		{

		}

		void Dispose()
		{

		}

		void Suspend()
		{

		}


		void PollClose()
		{
			FriContext->PollCloseEvent();
		}
	};
}

using namespace FlameEditorCLR;

EditorAppNative::EditorAppNative(void* hwnd, int width, int height)
{

	FRIRenderingContextDescription contextDescription;
	contextDescription.Width = width;
	contextDescription.Height = height;
	contextDescription.SampleCount = 0;
	contextDescription.RenderFramework = EFRIRendererFramework::DX11;
	contextDescription.IsFullscreen = false;
	contextDescription.WinContext = new Win32Context((HWND)hwnd, 0, 0, width, height, false);

	gameApp = new EditorEngineApp();
	gameApp->CreateContext(contextDescription);
	gameApp->Content.Connect("E:\\Dev\\FlameEngine\\Showcase\\Assets\\");

	gameApp->Load();

	gameApp->Renderer.AttachToScene(gameApp->CurrentScene);
}


void EditorAppNative::Frame()
{
	gameApp->Frame();
}


bool EditorAppNative::IsContextActive()
{
	return gameApp->IsContextActive();
}

void EditorAppNative::PollClose()
{
	gameApp->PollClose();
}