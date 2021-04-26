#pragma once


#include "Core/Common/CoreCommon.h"
#include "Core/Framework/Common/FString.h"
#include "Core/Framework/Common/FStack.h"
#include "Core/Framework/Globals/FGlobalID.h"
#include "../EntityComponent/EntityComponentSystem.h"

#include "../Physics/PhysicsComponent.h"
#include "../CameraSystem/CameraSystem.h"
#include "../ControlComponent.h"
#include "../Physics/PhysicsService.h"
#include "../Physics/PhysicsAllocator.h"
#include "../LightingSystem/LightComponent.h"
#include "../SkinnedMeshComponent.h"
#include "../Animation/AnimationComponent.h"

struct PhysicsSceneDescription
{
	PhysicsScene* pScene;
	PhysicsService* pService;
	PhysicsAllocator* pAllocator;

};


EXPORT(class,  Scene) : public IElementIdentifiable
{
public:
	Scene(PhysicsSceneDescription physDesc);
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



	template<typename...TComponents>
	FEntityComponentSystem<TComponents...> CreateSystem()
	{
		return FEntityComponentSystem<TComponents...>(&entityWorld);
	}


	FGlobalID GetID() const
	{
		return sceneID;
	}

	void LoadSystems();
	void Update();


	Entity Camera;
	Entity Sun;

	PhysicsAllocator* Physics;

private:

	PhysicsScene* physicsScene;
	PhysicsService* physicsService;

	FGlobalID sceneID;
	EntityWorld entityWorld;

};

