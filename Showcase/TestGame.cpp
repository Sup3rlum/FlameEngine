#include "TestGame.h"

#include "FlameEngine/Platform/Windows/Win32Context.h"

#include "FlameEngine/Core/Engine/ContentSystem/Client/AssetImportScripts/Mesh.h"
#include "FlameEngine/Core/Engine/ContentSystem/Client/AssetImportScripts/SkinnedMesh.h"
#include "FlameEngine/Core/Engine/ContentSystem/Client/AssetImportScripts/Material.h"
#include "FlameEngine/Core/Engine/ContentSystem/Client/AssetImportScripts/AnimationSequence.h"
#include "FlameEngine/Core/Engine/ContentSystem/Client/AssetImportScripts/PhysicsTriangleMesh.h"

TestGameApplication::TestGameApplication(FString appName, EFRIRendererFramework framework) : GameApplication(appName, framework, NULL)
{

	PerspMatrix = FPerspectiveMatrix(PI / 4, 16.0f / 9.0f, 0.1f, 300.0f);

	playerEntity = currentScene->CreateEntity<FTransform, CameraComponent, FirstPersonCharacterComponent, ControlComponent, CharacterBody>("player");
	currentScene->Camera = playerEntity;


	playerEntity.SetComponent(CameraComponent(PerspMatrix));
	playerEntity.Component<FTransform>().Position = FVector3(5, 5, 5);
	playerEntity.Component<FTransform>().Orientation = FQuaternion::Identity();

	playerEntity.Component<FirstPersonCharacterComponent>().yaw = 2.3;
	playerEntity.Component<FirstPersonCharacterComponent>().flySpeed = 100.0;

	playerEntity.SetComponent<CharacterBody>(currentScene->Physics->CreateCharacter(FVector3(0, 20, 0)));



	currentScene->Sun = currentScene->CreateEntity<DirectionalLightComponent>("Sun");
	currentScene->Sun.Component<DirectionalLightComponent>().Direction = FVector3::Normalize(FVector3(-1, -1, -1));
	currentScene->Sun.Component<DirectionalLightComponent>().Intensity = 5.0f;


}


void TestGameApplication::Update()
{



}



void TestGameApplication::Shoot()
{
	
	Entity ball = currentScene->CreateEntity<MeshComponent, MaterialComponent, FTransform, RigidBody>("ball");

	ball.SetComponent<MeshComponent>(ballMesh);
	ball.SetComponent<MaterialComponent>(defaultMaterial);

	ball.SetComponent<FTransform>(playerEntity.Component<FTransform>());
	ball.SetComponent<RigidBody>(currentScene->Physics->CreateDynamic(ball.Component<FTransform>()));
	ball.Component<RigidBody>().SetShape(PhysicsShape(PhysicsMaterial(0.8f, 0.8f, 0.1f), SphereGeometry(1.0f)));

	ball.Component<RigidBody>().SetLinearVelocity(playerEntity.Component<FirstPersonCharacterComponent>().LookDirection * 10.0f);
}

void TestGameApplication::Load()
{
	currentScene->RegisterSystem<TestPlayerSystem>(ECSExecutionFlag::MAIN_THREAD, FriContext);
	

	FMeshLoader modelLoader(false, true);
	FSkinnedMeshLoader skinnedModelLoader(false, true);
	FMaterialLoader materialLoader(false, true);
	FAnimSequenceLoader animLoader(false, true);
	FPhysTriangleMeshLoader trMeshloader(false, true);

	defaultMaterial = materialLoader.LoadFromLocal("materials/default.flmt", FriContext);
	ballMesh = modelLoader.LoadFromLocal("models/sphere.fl3d", FriContext);


	floorEntity = currentScene->CreateEntity<MeshComponent, MaterialComponent, FTransform, StaticRigidBody>("ground");


	floorEntity.SetComponent<MeshComponent>(modelLoader.LoadFromLocal("models/playground.fl3d", FriContext));
	floorEntity.SetComponent<MaterialComponent>(defaultMaterial);

	floorEntity.Component<MaterialComponent>().SetWrapMode(EMaterialWrap::Repeat);
	floorEntity.Component<MaterialComponent>().SetFilterMode(EMaterialFilter::Trilinear);

	floorEntity.SetComponent<FTransform>(FTransform());
	floorEntity.SetComponent<StaticRigidBody>(currentScene->Physics->CreateStatic(FTransform()));

	floorEntity.Component<StaticRigidBody>().SetShape(PhysicsShape(PhysicsMaterial(0.8f, 0.8f, 0.1f), currentScene->Physics->CookTriangleMeshGeometry(trMeshloader.LoadFromLocal("models/playground.fl3d"))));





	/*boneGuy = currentScene->CreateEntity<SkinnedMeshComponent, MaterialComponent, TransformComponent, AnimationComponent>("boneGuy");

	boneGuy.SetComponent<SkinnedMeshComponent>(skinnedModelLoader.LoadFromLocal("models/bone_guy.fl3d", FriContext));

	boneGuy.SetComponent<MaterialComponent>(materialLoader.LoadFromLocal("materials/default.flmt", FriContext));
	boneGuy.Component<MaterialComponent>().SetWrapMode(EMaterialWrap::Repeat);
	boneGuy.Component<MaterialComponent>().SetFilterMode(EMaterialFilter::Trilinear);

	boneGuy.SetComponent<TransformComponent>(FTransform());
	boneGuy.SetComponent<AnimationComponent>();

	boneGuy.Component<AnimationComponent>().AddSequence("Run", animLoader.LoadFromLocal("animations/anim_guy.fl3d.anim"));*/
}

void TestGameApplication::Suspend()
{


}

void TestGameApplication::Dispose()
{


}