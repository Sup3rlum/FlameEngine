#include "TestGame.h"
#include "TestPlayerScript.h"
#include "CoinScript.h"

#include "FlameEngine/Platform/Windows/Win32Context.h"
#include "FlameEngine/Core/Engine/ContentSystem/ImportScripts/Common.h"

#include "RenderModel.h"


TestGameApplication::TestGameApplication(const FString& appName) : GameApplication(appName)
{
	srand(static_cast <unsigned> (time(0)));

}


void TestGameApplication::Update(FGameTime gameTime)
{
	
}


void TestGameApplication::Load()
{

	PerspMatrix = FPerspectiveMatrix(PI / 3, FriContext->GetViewport().AspectRatio(), 0.1f, 500.0f);

	playerEntity = currentScene->CreateEntity<
		Camera,
		Input,
		CharacterBody,
		FPComponent,
		Behaviour>("player");

	playerEntity.Component<Camera>() = Camera(PerspMatrix);
	playerEntity.Transform().Position = FVector3(5, 5, 5);
	playerEntity.Transform().Orientation = FQuaternion::Identity();

	playerEntity.Component<CharacterBody>() = currentScene->CreateCharacterBody(FVector3(0, 30, 0));
	playerEntity.Component<Behaviour>().Attach<TestScript>(FriContext, this);

	defaultMaterial = Content.Load<Material>("Materials/default2.flmt");
	brickMaterial = Content.Load<Material>("Materials/brick_material.flmt");
	riverRockMaterial = Content.Load<Material>("Materials/river_rock.flmt");


	currentScene->Elements["GameCamera"] = playerEntity;

	currentScene->SceneLevel = Content.Load<Level>("Maps/map.flen", currentScene);

	currentScene->Elements["Sun"] = currentScene->CreateEntity<DirectionalLight>("Sun");
	currentScene->Elements["Sun"].Component<DirectionalLight>().Direction = FVector3::Normalize(FVector3(-1, -2.5f, -1));
	currentScene->Elements["Sun"].Component<DirectionalLight>().Color = Color::White;
	currentScene->Elements["Sun"].Component<DirectionalLight>().Intensity = 0.3f;

	currentScene->Elements["Environment"] = currentScene->CreateEntity<EnvironmentMap>("EnvMap");
	currentScene->Elements["Environment"].Component<EnvironmentMap>() = Content.Load<EnvironmentMap>("Materials/forest_hdr.flenv");

	auto coinEntity = currentScene->CreateEntity<Model, Behaviour>("Coin");

	coinEntity.Component<Model>().Mesh = Content.Load<Mesh>("Models/coin.fl3d");
	coinEntity.Component<Model>().Material = Content.Load<Material>("Materials/gold.flmt");
	coinEntity.Component<Behaviour>().Attach<CoinScript>(this);
	coinEntity.Transform().Position = FVector3(0, 5, 0);


	/*auto helmetEnt = currentScene->CreateEntity<Model>("helmet");

	helmetEnt.Component<Model>().Mesh = Content.Load<Mesh>("Models/helmet.fl3d");
	helmetEnt.Component<Model>().Material = Content.Load<Material>("Materials/helmet.flmt");
	helmetEnt.Transform().Position = FVector3(0, 1.0f, 0);
	helmetEnt.Transform().Orientation = FQuaternion::FromEulerAngles(0, PI, 0);*/

	auto helmetEnt = currentScene->CreateEntity<RenderObject>("helmet");

	auto renderList = new ModelRenderObject(FriContext);
	helmetEnt.Component<RenderObject>().list = renderList;

	renderList->model.Mesh = Content.Load<Mesh>("Models/helmet.fl3d");
	renderList->model.Material = Content.Load<Material>("Materials/helmet.flmt");

	helmetEnt.Transform().Position = FVector3(0, 1.0f, 0);
	helmetEnt.Transform().Orientation = FQuaternion::FromEulerAngles(0, PI, 0);

}
void TestGameApplication::Suspend()
{


}

void TestGameApplication::Dispose()
{


}

void TestGameApplication::Close()
{
	FriContext->PollCloseEvent();
}