#include "FlameCraft.h"
#include "PlayerScript.h"
#include "Renderer/WorldRenderer.h"

#include "FlameEngine/Platform/Windows/Win32Context.h"
#include "FlameEngine/Core/Engine/ContentSystem/ImportScripts/Common.h"

#include <FlameEngine/Core/Engine/GameSystem/Environment/ParticleSystem.h>
#include "World/SmokeParticle.h"


#include <FlameEngine/Core/Math/Calc/VectorField.h>

FlameCraft::FlameCraft(const FString& appName) : GameApplication(appName)
{
	srand(static_cast <unsigned> (time(0)));

}


void FlameCraft::Update(FGameTime gameTime)
{

}


void FlameCraft::Load()
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
	playerEntity.Component<Behaviour>().Attach<PlayerScript>(FriContext, this);

	currentScene->Elements["GameCamera"] = playerEntity;

	currentScene->SceneLevel = Content.Load<Level>("Maps/map.flen", currentScene);

	currentScene->Elements["Sun"] = currentScene->CreateEntity<DirectionalLight>("Sun");
	currentScene->Elements["Sun"].Component<DirectionalLight>().Direction = FVector3::Normalize(FVector3(-1, -2.5f, -1));
	currentScene->Elements["Sun"].Component<DirectionalLight>().Color = Color::White;
	currentScene->Elements["Sun"].Component<DirectionalLight>().Intensity = 5.0f;

	currentScene->Elements["Environment"] = currentScene->CreateEntity<EnvironmentMap>("EnvMap");
	currentScene->Elements["Environment"].Component<EnvironmentMap>() = Content.Load<EnvironmentMap>("Materials/forest_hdr.flenv");


	FRICommandList cmdList(FriContext->GetFRIDynamic());
	world.scene = currentScene;
	Entity worldRenderer = currentScene->CreateEntity<RenderObject>("worldRenderer");
	auto renderer = new WorldRenderer(FriContext, &world);
	world.Observers.Add(renderer);

	int renderDist = 8;

	for (auto x : FRange(-renderDist, renderDist))
	{
		for (auto z : FRange(-renderDist, renderDist))
		{
			world.GenerateChunk(IVector2(x, z));
		}
	}
	worldRenderer.Component<RenderObject>().list = renderer;
	playerEntity.Component<Behaviour>().ScriptAs<PlayerScript>()->_world = &world;

	CreateParticleSystem();
}
void FlameCraft::Suspend()
{


}

void FlameCraft::Dispose()
{


}

void FlameCraft::Close()
{
	FriContext->PollCloseEvent();
}

struct SmokePartMemory
{
	FMatrix4 world;
	FVector4 color;
};

void FlameCraft::CreateParticleSystem()
{
	FRICommandList cmdList(FriContext->GetFRIDynamic());

	auto smokesystem = currentScene->CreateEntity<ParticleManager>("Smoke Particle System");


	VectorField vectorField(AABB(FVector3(-1000), FVector3(1000)), [](const FVector3& vec)
		{
			FVector3 velocity(-vec.z, 0, vec.x);
			velocity = FVector3::NormalizeOrZero(velocity) * 2.0f;
			velocity.y = 2.0f;

			return velocity;
		});

	auto ps = new ParticleSystem<SmokeParticle>(FriContext, 1000);
	ps->SetParticleTick(
		[&](SmokeParticle& particle, float dt)
		{
			particle.Position.xyz += particle.Velocity.xyz * dt;

			auto vec = particle.Position.xyz;
			FVector3 velocity(-vec.z, 0, vec.x);
			velocity = FVector3::NormalizeOrZero(velocity) * 10.0f;
			velocity.y = 2.0f;


			particle.Velocity.xyz = velocity;
		}
	);


	ShaderLibrary psysLib = Content.Load<ShaderLibrary>("Shaders/smoke_particle.fslib");

	auto signatureShader = cmdList.GetDynamic()->CreateVertexShader(psysLib.Modules["SmokeParticle"].Parts[EFRIShaderType::Vertex].Memory);

	Material smokeTex = Content.Load<Material>("Materials/smoke_particle.flmt");
	Mesh smokeMesh = Content.Load<Mesh>("Models/quad.fl3d");
	smokeMesh.SetInstanceAttributes(cmdList,
		{
			FRIInputAttribute("INSTANCE_POS", 4, EFRIAttributeType::Float, EFRIBool::False, 48, 0, EFRIAttribUsage::PerInstance),
			FRIInputAttribute("INSTANCE_VEL", 4, EFRIAttributeType::Float, EFRIBool::False, 48, 16, EFRIAttribUsage::PerInstance),
			FRIInputAttribute("INSTANCE_DATA", 4, EFRIAttributeType::Float, EFRIBool::False, 48, 32, EFRIAttribUsage::PerInstance),

		}, signatureShader);

	ps->Shader = cmdList.GetDynamic()->CreateShaderPipeline(psysLib.Modules["SmokeParticle"]);
	ps->ParticleMesh = smokeMesh;
	ps->AddEmitter(ParticleEmitter<SmokeParticle>(FVector3(0, 25.0f, 0)));

	ps->Samplers.Add(FRISampler(0, smokeTex.GetMap(EMaterialMap::Diffuse).Handle));
	ps->Samplers.Add(FRISampler(1, smokeTex.GetMap(EMaterialMap::Normal).Handle));

	smokesystem.Component<ParticleManager>().ParticleSystemPtr = ps;

	delete signatureShader;
}