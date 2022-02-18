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

#include "../Environment/Level.h"
#include "Core/UX/UXContainer.h"
#include "../Behaviour.h"

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


	/*
	template<typename... TComponents>
	Entity CreateEntity(const FString& name, TComponents... args)
	{
		Entity e = CreateEntity<TComponents...>(name);
		(e.SetComponent(args), ...);
		return e;
	}*/

	template<typename... TComponents>
	Entity CreateEntity(const FString& name)
	{
		Entity entity = entityWorld.CreateEntityFromArchetype(name, TEntityArchetype<TComponents...>());

		//(entity.InitComponent<TComponents>(), ...);

		if (entity.HasComponent<Behaviour>())
			entity.Component<Behaviour>().pEntity = entity; 

		return entity;// entityWorld.CreateEntityFromArchetype(name, TEntityArchetype<TComponents...>());
	}

	template<typename TSystem, typename... TCreationArgs>
	TSystem* RegisterSystem(ECSExecutionFlag execFlags = ECSExecutionFlag::MAIN_THREAD, const TCreationArgs& ... args)
	{
		auto pSystem = new TSystem(args...);
		pSystem->scene = this;

		if (execFlags != ECSExecutionFlag::USER_TICK)
		{
			Systems.Add(pSystem);
		}
		return pSystem;
	}

	template<typename... TComponents>
	FEntityComponentSystem<TComponents...>* System()
	{
		auto pSystem = new FEntityComponentSystem<TComponents...>(this);
		return pSystem;
	}

	void RegisterParticleSystem(ParticleSystemBase* particleSystem, ParticleRenderer* particleRenderer);

	AABB GetAABB() const;
	FGlobalID GetID() const;

	void LoadSystems();
	void Update(FGameTime gameTime);
	void UpdateDirectionalLights();

	Entity Camera;
	Entity Sun;
	Level SceneLevel;

	PhysicsAllocator* Physics;
	FArray<FKeyVal<ParticleSystemBase*, ParticleRenderer*>> ParticleSystems;

	UXContainer* uxContainer;

private:

	FArray<FEntityComponentSystemBase*> Systems;
	PhysicsScene* physicsScene;
	PhysicsService* physicsService;

	FRIContext* FriContext;

	FGlobalID sceneID;
	EntityWorld entityWorld;
	
	template<typename ...TComponents>
	friend class FEntityComponentSystem;
};

