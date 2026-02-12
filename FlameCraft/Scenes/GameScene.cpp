
#include "../Renderer/WorldRenderer.h"
#include "GameScene.h"

#include "../PlayerScript.h"

#include "FlameEngine/Platform/Windows/Win32Context.h"
#include "FlameEngine/Core/Engine/ContentSystem/ImportScripts/Common.h"
#include "FlameEngine/Core/Engine/Renderer/Environment/SkySphere.h"


void GameScene::OnCreateResources(FRIContext* FriContext, FAssetManager& Content)
{
	PerspMatrix = FPerspectiveMatrix(PI / 3, FriContext->GetViewport().AspectRatio(), 0.1f, 500.0f);

	playerEntity = CreateEntity<
		Camera,
		Input,
		CharacterBody,
		FPComponent,
		Behaviour>("player");

	playerEntity.Component<Camera>() = Camera(PerspMatrix);
	playerEntity.Transform().Position = FVector3(5, 5, 5);
	playerEntity.Transform().Orientation = FQuaternion::Identity();

	playerEntity.Component<CharacterBody>() = CreateCharacterBody(FVector3(0, 50, 0));
	playerEntity.Component<Behaviour>().AttachNew<PlayerScript>(FriContext, this);

	Elements["GameCamera"] = playerEntity;

	Elements["Sun"] = CreateEntity<DirectionalLight>("Sun");
	Elements["Sun"].Component<DirectionalLight>().Direction = FVector3::Normalize(FVector3(-1, -2.5f, -1));
	Elements["Sun"].Component<DirectionalLight>().Color = Color::White;
	Elements["Sun"].Component<DirectionalLight>().Intensity = 5.0f;

	/*Elements["Environment"] = CreateEntity<EnvironmentMap>("EnvMap");
	Elements["Environment"].Component<EnvironmentMap>() = Content.Load<EnvironmentMap>("Materials/pure_sky.flenv");

	auto mike = CreateEntity<Model>("Mike");
	mike.Component<Model>().Mesh = Content.Load<Mesh>("Models/mike.fl3d");
	mike.Component<Model>().Material = Content.Load<Material>("Materials/mike.flmt");
	mike.Transform().Position = FVector3(30, 24, -2);

	world.scene = this;
	Entity worldRenderer = CreateEntity<RenderObject>("worldRenderer");
	auto renderer = new WorldRenderer(FriContext, &world);
	world.Observers.Add(renderer);

	int renderDist = 16;

	for (auto x : FRange(-renderDist, renderDist))
	{
		for (auto z : FRange(-renderDist, renderDist))
		{
			world.GenerateChunk(IVector2(x, z));
		}
	}
	//worldRenderer.Component<RenderObject>().RenderList = renderer;
	playerEntity.Component<Behaviour>().ScriptAs<PlayerScript>()->_world = &world;*/
}

void GameScene::OnDestroyResources()
{

}