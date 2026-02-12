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
#include "../Mesh.h"
#include "../Animation/AnimationComponent.h"
#include "../../Renderer/Environment/ParticleRenderer.h"

#include "../Environment/Level.h"
#include "Core/UX/UXContainer.h"
#include "../Behaviour.h"
#include "Core/Engine/ContentSystem/Client/LocalAssetManager.h"

#include "SceneDefinitions.h"

enum class ECSExecutionFlag : uint32
{
	USER_TICK = 1,
	MAIN_THREAD = 2
};




EXPORT(class,  Scene) : IProperties
{
public:
	Scene(FString Name, class GameApplication* Game, FRIContext* renderContext, PhysicsDescription physDesc);
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
			entity.Component<Behaviour>().pGame = Game;
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

	void Update(FGameTime gameTime);
	void UpdateSystems();
	void UpdateBehaviour(FGameTime gameTime);
	void UpdateDirectionalLights();
	void FinishUpdate();
	virtual void OnCreateResources(FRIContext* FriContext, FAssetManager& Content) = 0;
	virtual void OnDestroyResources() = 0;

	FVector3 Raycast(FRay ray);

	/* Physics */

	CharacterBody CreateCharacterBody(FTransform transform);
	RigidBody CreateRigidBody(FTransform transform);
	StaticRigidBody CreateStaticRigidBody(FTransform transform);
	TriangleMeshGeometry CookTriangleMeshGeometry(PhysicsTriangleMeshDesc desc);
	PhysicsAllocator* PhysicsWorld();

	PropertyEnum(SceneBackgroundMode, SceneBackground, SceneBackgroundMode::EnvironmentMap);

	// EnvironmentMap Properties
	PropertyEnum(EnvironmentMapSelectionMode, EnvironmentMapSelection, EnvironmentMapSelectionMode::First);
	PropertyColor32(BackgroundColor, Color32::CornflowerBlue)
	

	FHashMap<FString, Entity> Elements;
	FString Name;
	UXContainer* uxContainer;

private:
	PhysicsAllocator* Physics;

	FArray<FEntityComponentSystemBase*> Systems;
	PhysicsScene* physicsScene;
	PhysicsService* physicsService;

	FRIContext* FriContext;
	EntityWorld EntWorld;
	class GameApplication* Game;
	
	template<typename ...TComponents>
	friend class FEntityComponentSystem;

};

