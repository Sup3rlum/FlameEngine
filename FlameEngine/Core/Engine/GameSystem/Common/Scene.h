#pragma once


#include "Core/Common/CoreCommon.h"
#include "Core/Framework/Common/FString.h"
#include "Core/Framework/Common/FStack.h"
#include "Core/Framework/Globals/FGlobalID.h"
#include "../EntityComponent/EntityComponentSystem.h"
#include "../EntityComponent/Entity.h"

#include "../Physics/PhysicsComponent.h"
#include "../CameraSystem/CameraComponent.h"
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


enum class ECSExecutionFlag : uint32
{
	USER_TICK = 1,
	MAIN_THREAD = 2
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

	FArray<FEntityComponentSystemBase*> Systems;

	PhysicsScene* physicsScene;
	PhysicsService* physicsService;

	FGlobalID sceneID;
	EntityWorld entityWorld;


	uint64 UpdateFrameNum;
	float UpdateFrameDelta;
	uint64 LastFrameTimestamp;
	uint64 FrameTimestamp;

};

