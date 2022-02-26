#include "TestGame.h"
#include "TestPlayerScript.h"

#include "FlameEngine/Platform/Windows/Win32Context.h"

#include "FlameEngine/Core/Engine/ContentSystem/Client/AssetImportScripts/Mesh.h"
#include "FlameEngine/Core/Engine/ContentSystem/Client/AssetImportScripts/SkinnedMesh.h"
#include "FlameEngine/Core/Engine/ContentSystem/Client/AssetImportScripts/Material.h"
#include "FlameEngine/Core/Engine/ContentSystem/Client/AssetImportScripts/AnimationSequence.h"
#include "FlameEngine/Core/Engine/ContentSystem/Client/AssetImportScripts/PhysicsTriangleMesh.h"
#include "FlameEngine/Core/Engine/ContentSystem/Client/AssetImportScripts/InstanceMesh.h"
#include "FlameEngine/Core/Engine/ContentSystem/Client/AssetImportScripts/Level.h"



TestGameApplication::TestGameApplication(FString appName, EFRIRendererFramework framework) : GameApplication(appName, framework, NULL)
{
	srand(static_cast <unsigned> (time(0)));

	PerspMatrix = FPerspectiveMatrix(PI / 3, 16.0f / 9.0f, 0.1f, 300.0f);

	auto playerEntity = currentScene->CreateEntity<
		FTransform, 
		CameraComponent, 
		ControlComponent, 
		CharacterBody,
		FPComponent,
		Behaviour>("player");


	currentScene->Camera = playerEntity;


	playerEntity.SetComponent(CameraComponent(PerspMatrix));
	playerEntity.Component<FTransform>().Position = FVector3(5, 5, 5);
	playerEntity.Component<FTransform>().Orientation = FQuaternion::Identity();

	playerEntity.SetComponent<CharacterBody>(currentScene->Physics->CreateCharacter(FVector3(0, 30, 0)));

	playerEntity.Component<Behaviour>().Attach<TestScript>(FriContext, this);

	currentScene->Sun = currentScene->CreateEntity<DirectionalLight>("Sun");
	currentScene->Sun.Component<DirectionalLight>().Direction = FVector3::Normalize(FVector3(-1, -2.5f, -1));
	currentScene->Sun.Component<DirectionalLight>().Color = Color::White;
	currentScene->Sun.Component<DirectionalLight>().Intensity = 5.0f;
}


void TestGameApplication::Update(FGameTime gameTime)
{
	
}



void TestGameApplication::Shoot()
{

	/*for (int i = 0; i < 400; i++)
	{
		Entity ball = currentScene->CreateEntity<Mesh, Material, FTransform, RigidBody>("ball");

		ball.SetComponent<Mesh>(ballMesh);
		ball.SetComponent<Material>(riverRockMaterial);

		auto start = playerEntity.Component<FTransform>();

		start.Position += FVector3((i%20)*2.1f - 10, 0, (i/20)*2.1f - 10);

		ball.SetComponent<FTransform>(start);
		ball.SetComponent<RigidBody>(currentScene->Physics->CreateDynamic(ball.Component<FTransform>()));

		ball.Component<RigidBody>().SetShape(
			PhysicsShape(
				PhysicsMaterial(0.8f, 0.8f, 0.1f),
				BoxGeometry(1.0f)
			)
		);

		ball.Component<RigidBody>().SetLinearVelocity(playerEntity.Component<FPComponent>().LookDirection * 10.0f);
	}*/
}

void TestGameApplication::Load()
{

	CreateParticleSystem();
	ballMesh = Content.Load<Mesh>("Models/cube.fl3d", FriContext);

	defaultMaterial =	Content.Load<Material>("Materials/default2.flmt", FriContext);
	brickMaterial =		Content.Load<Material>("Materials/brick_material.flmt", FriContext);
	riverRockMaterial = Content.Load<Material>("Materials/river_rock.flmt", FriContext);

	currentScene->SceneLevel = Content.Load<Level>("Maps/dust2.flen", FriContext, currentScene);


	/*boneGuy = currentScene->CreateEntity<SkinnedMesh, Material, FTransform, AnimationComponent>("boneguy");


	boneGuy.SetComponent<SkinnedMesh>(FLocalContent::LoadFromLocal<SkinnedMesh>("animations/anim_guy.fl3d.anim"));
	boneGuy.Component<AnimationComponent>().AddSequence("run", FLocalContent::LoadFromLocal<AnimationSequence>("animations/anim_guy.fl3d.anim"));
	boneGuy.SetComponent<Material>(graniteMaterial);
	boneGuy.SetComponent<FTransform>(FTransform());*/



    /*/benchEntity = currentScene->CreateEntity<Mesh, Material, FTransform, StaticRigidBody>("ground");


	benchEntity.SetComponent<Mesh>(FLocalContent::LoadFromLocal<Mesh>("models/corridor.fl3d", FriContext));
	benchEntity.SetComponent<Material>(metalMaterial);

	benchEntity.Component<Material>().SetWrapMode(EMaterialWrap::Repeat);
	benchEntity.Component<Material>().SetFilterMode(EMaterialFilter::Trilinear);
	benchEntity.SetComponent<FTransform>(FTransform(FVector3(0,0,0)));
	benchEntity.SetComponent<StaticRigidBody>(currentScene->Physics->CreateStatic(FTransform(FVector3(0, 0, 0))));

	benchEntity.Component<StaticRigidBody>().SetShape(PhysicsShape(PhysicsMaterial(0.8f, 0.8f, 0.1f), currentScene->Physics->CookTriangleMeshGeometry(FLocalContent::LoadFromLocal<PhysicsTriangleMeshDesc>("models/corridor.fl3d"))));*/



	/*Entity redLight = currentScene->CreateEntity<PointLight>("RedLight");
	redLight.Component<PointLight>().Position = FVector3(0, 3, 0);
	redLight.Component<PointLight>().Color = Color::Red;
	redLight.Component<PointLight>().Intensity = 10;
	redLight.Component<PointLight>().Radius = 5;

	Entity greenLight = currentScene->CreateEntity<PointLight>("GreenLight");
	greenLight.Component<PointLight>().Position = FVector3(0, 3, 0);
	greenLight.Component<PointLight>().Color = Color::Green;
	greenLight.Component<PointLight>().Intensity = 10;
	greenLight.Component<PointLight>().Radius = 5;
	
	Entity blueLight = currentScene->CreateEntity<PointLight>("BlueLight");
	blueLight.Component<PointLight>().Position = FVector3(0, 10, 0);
	blueLight.Component<PointLight>().Color = Color::Blue;
	blueLight.Component<PointLight>().Intensity = 20;
	blueLight.Component<PointLight>().Radius = 5;*/
	/*
	Entity whiteLight = currentScene->CreateEntity<SpotLight>("WhiteLight");
	whiteLight.Component<SpotLight>().Position = FVector3(1, 7, 0);
	whiteLight.Component<SpotLight>().Direction = FVector3::Normalize(FVector3(-1, -0.5, -1));
	whiteLight.Component<SpotLight>().Color = Color::White;
	whiteLight.Component<SpotLight>().Intensity = 0;
	whiteLight.Component<SpotLight>().Radius = 10;
	whiteLight.Component<SpotLight>().ApertureSize = 0.5;
	whiteLight.Component<SpotLight>().ApertureSharpness = 4;*/



}

void TestGameApplication::CreateParticleSystem()
{
	/* Test Particle System */

	TestParticleSystem* testParticleSystem = new TestParticleSystem(FriContext, 500);

	testParticleSystem->SetParticleTick([](TestParticle& particle, float dt)
		{
			particle.Position += particle.Velocity * dt;
			particle.Velocity += FVector3(0, -10, 0) * dt;

			if (particle.Color.g > 0)
			{
				particle.Color.g -= dt / 2;
			}

			if (particle.Color.g <= 0.01)
			{
				if (particle.Color.r > 0)
				{
					particle.Color.r -= dt / 2;
				}
			}
		});

	testParticleSystem->SetStageTransform([](TestParticle& particle)
		{
			return TestParticleStage(FTranslationMatrix(particle.Position), particle.Color);
		});


	testParticleSystem->AddEmitter(TestEmitter(FVector3(0, 5, 0)));

	/* Test Particle System renderer */
	ShaderLibrary psysLib = Content.Load<ShaderLibrary>("Shaders/testParticle_dx.fslib", FriContext);

	FArray<FRIVertexDeclarationComponent> vertexDecl;
	vertexDecl.Add(FRIVertexDeclarationComponent("POSITION", 3, EFRIVertexDeclerationAttributeType::Float, EFRIBool::False, 56, 0));
	vertexDecl.Add(FRIVertexDeclarationComponent("NORMAL", 3, EFRIVertexDeclerationAttributeType::Float, EFRIBool::False, 56, 12));
	vertexDecl.Add(FRIVertexDeclarationComponent("TANGENT", 3, EFRIVertexDeclerationAttributeType::Float, EFRIBool::False, 56, 24));
	vertexDecl.Add(FRIVertexDeclarationComponent("BITANGENT", 3, EFRIVertexDeclerationAttributeType::Float, EFRIBool::False, 56, 36));
	vertexDecl.Add(FRIVertexDeclarationComponent("TEXCOORD", 2, EFRIVertexDeclerationAttributeType::Float, EFRIBool::False, 56, 48));

	FArray<FRIVertexDeclarationComponent> instanceDecl;
	instanceDecl.Add(FRIVertexDeclarationComponent(FRIInputSemantic("INSTANCE_WORLD", 0), 4, EFRIVertexDeclerationAttributeType::Float, EFRIBool::False, 80, 0, EFRIAttribUsage::PerInstance));
	instanceDecl.Add(FRIVertexDeclarationComponent(FRIInputSemantic("INSTANCE_WORLD", 1), 4, EFRIVertexDeclerationAttributeType::Float, EFRIBool::False, 80, 16, EFRIAttribUsage::PerInstance));
	instanceDecl.Add(FRIVertexDeclarationComponent(FRIInputSemantic("INSTANCE_WORLD", 2), 4, EFRIVertexDeclerationAttributeType::Float, EFRIBool::False, 80, 32, EFRIAttribUsage::PerInstance));
	instanceDecl.Add(FRIVertexDeclarationComponent(FRIInputSemantic("INSTANCE_WORLD", 3), 4, EFRIVertexDeclerationAttributeType::Float, EFRIBool::False, 80, 48, EFRIAttribUsage::PerInstance));
	instanceDecl.Add(FRIVertexDeclarationComponent("INSTANCE_COLOR", 4, EFRIVertexDeclerationAttributeType::Float, EFRIBool::False, 80, 64, EFRIAttribUsage::PerInstance));


	Material smokeMap = Content.Load<Material>("Materials/smoke.flmt", FriContext);
	InstanceMesh instancedBallMesh = Content.Load<InstanceMesh>("Models/quad.fl3d", FriContext, vertexDecl, instanceDecl, psysLib.Modules["Test"].Parts[EFRIResourceShaderType::Vertex]);

	ParticleRenderer* testParticleRenderer = new ParticleRenderer(FriContext, psysLib.Modules["Test"]);

	testParticleRenderer->SetInstanceMesh(instancedBallMesh);
	testParticleRenderer->Samplers.Add(FUniformSampler(0, smokeMap.GetMap(EMaterialMap::Diffuse).Handle));

	currentScene->RegisterParticleSystem(testParticleSystem, testParticleRenderer);

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