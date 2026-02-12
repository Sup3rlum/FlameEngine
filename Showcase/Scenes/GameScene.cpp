

#include "GameScene.h"

#include "../GameScripts/PlayerScript.h"
#include "../GameScripts/PlanetScript.h"

#include "FlameEngine/Platform/Windows/Win32Context.h"
#include "FlameEngine/Core/Engine/ContentSystem/ImportScripts/Common.h"
#include "FlameEngine/Core/Engine/Renderer/Environment/SkySphere.h"

#include "../Planet/Planet.h"

void GameScene::OnCreateResources(FRIContext* FriContext, FAssetManager& Content)
{
	PerspMatrix = FPerspectiveMatrix(PI / 2, FriContext->GetViewport().AspectRatio(), 0.01f, 200.0f);

	auto playerEntity = CreateEntity<
		Camera,
		Input,
		Behaviour,
		FPComponent>("player");

	playerEntity.Component<Camera>() = Camera(PerspMatrix);
	playerEntity.Transform().Position = FVector3(0, 0, -5);
	playerEntity.Transform().Orientation = FQuaternion::Identity();
	playerEntity.Component<Behaviour>().AttachNew<PlayerScript>(FriContext, this);
	Elements["GameCamera"] = playerEntity;

	Elements["Sun"] = CreateEntity<DirectionalLight>("Sun");
	Elements["Sun"].Component<DirectionalLight>().Direction = FVector3::Normalize(FVector3(1, -1, 0.3f));
	Elements["Sun"].Component<DirectionalLight>().Color = Color::White;
	Elements["Sun"].Component<DirectionalLight>().Intensity = 10.0f;

	Elements["Environment"] = CreateEntity<EnvironmentMap>("EnvMap");
	Elements["Environment"].Component<EnvironmentMap>() = Content.Load<EnvironmentMap>("Materials/pure_sky.flenv");

	auto planetMesh = new Planet(FriContext);
	//planetMesh->Generate();

	auto planet = CreateEntity<RenderObject, Behaviour>("Planet");
	planet.Component<RenderObject>().RenderList = planetMesh;
	planet.Component<Behaviour>().AttachNew<PlanetScript>(FriContext, this);

	uxContainer->LoadURL("file:///welcome.html");

	typedef FDelegate<void(float, float, float, float, float, float)> SetNoiseParamsFunc;
	typedef FDelegate<void(int, float, float, float)> SetCraterParamsFunc;
	uxContainer->SetCallback("SetOceanNoiseParams",			SetNoiseParamsFunc::Make<Planet, &Planet::SetOceanNoiseParams>(planetMesh));
	uxContainer->SetCallback("SetContinentNoiseParams",		SetNoiseParamsFunc::Make<Planet, &Planet::SetContinentNoiseParams>(planetMesh));
	uxContainer->SetCallback("SetMountainMaskNoiseParams",	SetNoiseParamsFunc::Make<Planet, &Planet::SetMountainMaskNoiseParams>(planetMesh));
	uxContainer->SetCallback("SetCraterParams",				SetCraterParamsFunc::Make<Planet, &Planet::SetCraterParams>(planetMesh));

}
void GameScene::OnDestroyResources()
{
	
}