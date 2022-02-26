#pragma once


#include "EntityMemoryStack.h"


struct EntityWorld;


struct GEntityID
{
	union
	{
		struct { FEntityMemoryStack::FEntityMemoryBlock* Block; uint64 Index; };
		struct { uint64 High, Low; };
		struct { void* HighPtr, * LowPtr; };
	};


	class Entity& GetControl();

	bool IsValid();
};


EXPORT(struct, Entity)
{


	template<typename TComponent>
	void SetComponent(const TComponent& component)
	{
		EntityId->Block->GetComponent<TComponent>(EntityId->Index) = component;
	}

	template<typename TComponent, typename...TCreationArgs>
	void InitComponent(const TCreationArgs& ... args)
	{
		new (&EntityId->Block->GetComponent<TComponent>(EntityId->Index)) TComponent(args...);
	}


	template<typename TComponent>
	TComponent& Component()
	{
		return EntityId->Block->GetComponent<TComponent>(EntityId->Index);
	}


	template<typename TComponent>
	const TComponent& Component() const
	{
		return EntityId->Block->GetComponent<TComponent>(EntityId->Index);
	}

	template<typename TComponent>
	bool HasComponent() const
	{
		return GetArchetype().GetIndex<TComponent>() != -1;
	}

	template<typename TComponent>
	void AddComponent() 
	{
		if (!HasComponent<TComponent>())
		{
			
		}
	}

	const FEntityArchetype& GetArchetype() const;
	uint32 GetRefCount() const;

	FString GetName() const;
	bool IsValid() const;


	Entity(GEntityID* eid, const FString& name);
	Entity(Entity&& other) noexcept;
	Entity(const Entity& other);
	Entity& operator=(Entity&& other) noexcept;
	void operator=(const Entity& other);

	~Entity();

private:

	Entity();

	friend class EntityManager;
	friend class FEntityMemoryStack;
	friend class Scene;

	FSharedPointer<GEntityID> EntityId;
	FString Name;
};