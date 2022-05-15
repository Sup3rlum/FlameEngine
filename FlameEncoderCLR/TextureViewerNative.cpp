#include "TextureViewerNative.h"

#include <Platform/Windows/Win32Context.h>
#include <Core/Engine/GameSystem/Game.h>
#include <Core/Engine/GameSystem/Mesh.h>

#include "Core/Engine/ContentSystem/ImportScripts/Mesh.h"
#include "Core/Engine/ContentSystem/ImportScripts/Material.h"
#include "Core/Engine/ContentSystem/ImportScripts/EnvironmentMap.h"


namespace FlameEncoderCLR
{
	class TextureViewerEngine : public GameApplication
	{

		Entity sampleSphere;

	public:
		TextureViewerEngine() :
			GameApplication("TextureViewer")
		{

		}

		void Load() override
		{
			auto PerspMatrix = FPerspectiveMatrix(PI / 9, FriContext->GetViewport().AspectRatio(), 0.1f, 300.0f);

			Entity cameraEnt = currentScene->CreateEntity<Camera>("camera");

			cameraEnt.Component<FTransform>().Position = FVector3(5, 1, 5);
			cameraEnt.Component<FTransform>().Orientation = FQuaternion::FromEulerAngles(FVector3(0, PI, 0));
			cameraEnt.Component<Camera>() = Camera(FViewMatrix(cameraEnt.Component<FTransform>().Position, FVector3(0), FVector3(0,1,0)), PerspMatrix);

			currentScene->Elements["GameCamera"] = cameraEnt;

			currentScene->Elements["Sun"] = currentScene->CreateEntity<DirectionalLight>("Sun");
			currentScene->Elements["Sun"].Component<DirectionalLight>().Direction = FVector3::Normalize(FVector3(0, -1.0f, -1.0f));
			currentScene->Elements["Sun"].Component<DirectionalLight>().Color = Color::White;
			currentScene->Elements["Sun"].Component<DirectionalLight>().Intensity = 5.0f;


			sampleSphere = currentScene->CreateEntity<Model>("sampleSphere");
			sampleSphere.Component<Model>().Mesh = Content.Load<Mesh>("Models/uvSphere.fl3d");
			sampleSphere.Component<Model>().Material = Content.Load<Material>("Materials/gold.flmt");
			sampleSphere.Component<FTransform>() = FTransform();


			currentScene->Elements["Environment"] = currentScene->CreateEntity<EnvironmentMap>("EnvMap");
			currentScene->Elements["Environment"].Component<EnvironmentMap>() = Content.Load<EnvironmentMap>("Materials/forest_hdr.flenv");

		}		

		void RenderIrradiance(FRICreationDescriptor* data)
		{
			/*auto sp = Renderer.RenderIrradiance(FriContext, data);
			currentScene->Elements["Environment"].Component<EnvironmentMap>().Specular = sp;*/
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

		void SetMaterial(FArray<FKeyVal<IVector2,FRICreationDescriptor>> data)
		{
			Material::MapCollection maps;

			int counter = 0;
			for (auto [mapDesc, mapData] : data)
			{
				auto friTex = FriContext->GetFRIDynamic()->CreateTexture2D(
					mapDesc.x,
					mapDesc.y,
					0,
					EFRITextureFormat::RGBA8UNORM,
					FRIColorDataFormat(EFRIChannels::RGBA, EFRIPixelStorage::UnsignedByte),
					mapData
				);

				maps[counter++] = MaterialMap(friTex);
			}

			sampleSphere.Component<Model>().Material = Material(maps, FriContext);
		}

		void PollClose()
		{
			FriContext->PollCloseEvent();
		}
	};
}

using namespace FlameEncoderCLR;

TextureViewerNative::TextureViewerNative(void* hwnd, int width, int height)
{

	FRIRenderingContextDescription contextDescription;
	contextDescription.Width = width;
	contextDescription.Height = height;
	contextDescription.SampleCount = 0;
	contextDescription.RenderFramework = EFRIRendererFramework::DX11;
	contextDescription.IsFullscreen = false;
	contextDescription.WinContext = new Win32Context((HWND)hwnd, 0, 0, width, height, false);

	gameApp = new TextureViewerEngine();
	gameApp->CreateContext(contextDescription);
	gameApp->Content.Connect("E:\\Dev\\FlameEngine\\Showcase\\Assets\\");

	gameApp->Load();

	gameApp->Renderer.AttachToScene(gameApp->currentScene);
}


void TextureViewerNative::Frame()
{
	gameApp->Frame();
}


bool TextureViewerNative::IsContextActive()
{
	return gameApp->IsContextActive();
}

void TextureViewerNative::PollClose()
{
	gameApp->PollClose();
}


void TextureViewerNative::SetMaterial(NativeTextureDesc* textures, int num)
{
	FArray<FKeyVal<IVector2, FRICreationDescriptor>> data;

	for (int i = 0; i < num; i++)
	{
		data.Add(FKeyVal<IVector2, FRICreationDescriptor>(IVector2(textures[i].width, textures[i].height), FRICreationDescriptor(textures[i].data, textures[i].dataSize)));
	}

	gameApp->SetMaterial(data);
}

void TextureViewerNative::RenderIrradiance(FRIData* data)
{
	FRICreationDescriptor desc[6];

	for (int i = 0; i < 6; i++)
	{
		desc[i] = FRICreationDescriptor(data[i].data, data[i].ByteSize);
	}
	
	gameApp->RenderIrradiance(desc);
}