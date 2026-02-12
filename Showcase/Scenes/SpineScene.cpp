

#include "SpineScene.h"

#include "../GameScripts/PlayerScript.h"

#include "FlameEngine/Platform/Windows/Win32Context.h"
#include "FlameEngine/Core/Engine/ContentSystem/ImportScripts/Common.h"
#include "FlameEngine/Core/Engine/Renderer/Environment/SkySphere.h"

#include "../Spine/Spine.h"

void SpineScene::OnCreateResources(FRIContext* FriContext, FAssetManager& Content)
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

	auto spineMesh = new Spine(FriContext);
	//planetMesh->Generate();

	auto planet = CreateEntity<RenderObject, Behaviour>("Spine");
	planet.Component<RenderObject>().RenderList = spineMesh;

}
void SpineScene::OnDestroyResources()
{

}