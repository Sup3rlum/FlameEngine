#pragma once


#include "Core/Common/CoreCommon.h"
#include "Core/Framework/Common/FString.h"
#include "Core/Framework/Common/FStack.h"
#include "Core/Framework/Globals/FGlobalID.h"
#include "../EntityComponent/EntityComponentSystem.h"
#include "../EntityComponent/Entity.h"
#include "../Environment/ParticleSystem.h"

#include "../Physics/PhysicsComponent.h"
#include "../CameraSystem/CameraComponent.h"
#include "../ControlComponent.h"
#include "../Physics/PhysicsService.h"
#include "../Physics/PhysicsAllocator.h"
#include "../LightingSystem/LightComponent.h"
#include "../SkinnedMesh.h"
#include "../Animation/AnimationComponent.h"
#include "../../Renderer/ParticleRenderer.h"

#include "Core/UX/UXContainer.h"

struct PhysicsSceneDescription
{
	PhysicsScene* pScene;
	PhysicsService* pService;
	PhysicsAllocator* pAllocator;

};


enum class ECSExecutionFlag : uint32
{
	USER_TICK = 1,
	MAIN_THREAD = 2
};


EXPORT(class,  Scene) : public IElementIdentifiable
{
public:
	Scene(PhysicsSceneDescription physDesc, FRIContext* renderContext);
	~Scene();



	template<typename... TComponents>
	Entity CreateEntity(TComponents... args)
	{
		Entity e = CreateEntity<TComponents...>();
		(e.SetComponent(args), ...);
		return e;
	}

	template<typename... TComponents>
	Entity CreateEntity(const FString& name)
	{
		return entityWorld.CreateEntityFromArchetype(name, TEntityArchetype<TComponents...>());
	}


	template<typename TSystem, typename... TCreationArgs>
	TSystem* RegisterSystem(ECSExecutionFlag execFlags = ECSExecutionFlag::MAIN_THREAD, const TCreationArgs& ... args)
	{
		auto systPtr = new TSystem(args...);

		systPtr->scene = this;
		systPtr->entityWorld = &entityWorld;

		if (execFlags != ECSExecutionFlag::USER_TICK)
		{
			Systems.Add(systPtr);
		}
		return systPtr;
	}

	void RegisterParticleSystem(ParticleSystemBase* particleSystem, ParticleRenderer* particleRenderer)
	{
		ParticleSystems.Add(FKeyVal<ParticleSystemBase*, ParticleRenderer*>(particleSystem, particleRenderer));
	}


	FGlobalID GetID() const
	{
		return sceneID;
	}

	void LoadSystems();
	void Update(FGameTime gameTime);

	Entity Camera;
	Entity Sun;

	PhysicsAllocator* Physics;
	FArray<FKeyVal<ParticleSystemBase*, ParticleRenderer*>> ParticleSystems;

	UXContainer* uxContainer;

	PointLight pointLights[4];

private:

	FArray<FEntityComponentSystemBase*> Systems;
	PhysicsScene* physicsScene;
	PhysicsService* physicsService;

	FRIContext* FriContext;

	FGlobalID sceneID;
	EntityWorld entityWorld;
};

