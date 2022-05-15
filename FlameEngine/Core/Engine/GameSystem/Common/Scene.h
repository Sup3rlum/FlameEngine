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
#include "../InputComponent.h"
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


EXPORT(class,  Scene)
{
public:
	Scene(PhysicsSceneDescription physDesc, FRIContext* renderContext);
	~Scene();

	template<typename... TComponents>
	Entity CreateEntity(const FString& name)
	{
		Entity entity = EntWorld.CreateEntityFromArchetype(name, TEntityArchetype<FTransform, TComponents...>());

		//(entity.InitComponent<TComponents>(), ...);

		if (entity.HasComponent<Behaviour>())
		{
			entity.Component<Behaviour>().pEntity = entity;
			entity.Component<Behaviour>().pScene = this;
		}
		return entity;
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


	FArray<Entity> QueryEntities(const FString& name);


	AABB GetAABB() const;

	void LoadSystems();
	void Update(FGameTime gameTime);
	void UpdateSystems();
	void UpdateBehaviour(FGameTime gameTime);
	void UpdateDirectionalLights();
	void FinishUpdate();

	/* Physics */

	CharacterBody CreateCharacterBody(FTransform transform);
	RigidBody CreateRigidBody(FTransform transform);
	StaticRigidBody CreateStaticRigidBody(FTransform transform);
	TriangleMeshGeometry CookTriangleMeshGeometry(PhysicsTriangleMeshDesc desc);

	Level SceneLevel;
	FHashMap<FString, Entity> Elements;

	UXContainer* uxContainer;

private:
	PhysicsAllocator* Physics;

	FArray<FEntityComponentSystemBase*> Systems;
	PhysicsScene* physicsScene;
	PhysicsService* physicsService;

	FRIContext* FriContext;

	EntityWorld EntWorld;
	
	template<typename ...TComponents>
	friend class FEntityComponentSystem;
public:
	FTimeSpan physTime, dynTime, behTime;
};

