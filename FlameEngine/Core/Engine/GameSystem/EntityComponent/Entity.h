#pragma once


#include "EntityMemoryStack.h"
#include "Core/Math/Module.h"


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


enum class TransformMode
{
	Absolute,
	Relative
};

EXPORT(struct, Entity)
{
	FArray<struct Entity> Children;

	template<typename TComponent>
	void SetComponent(const TComponent& component)
	{
		Memory::Copy(&EntityId->Block->GetComponent<TComponent>(EntityId->Index), &component, sizeof(TComponent));
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

	FTransform& Transform()
	{
		return Component<FTransform>();
	}

	const FTransform& Transform() const
	{
		return Component<FTransform>();
	}

	const FEntityArchetype& GetArchetype() const;
	uint32 GetRefCount() const;

	FString GetName() const;
	bool IsValid() const;

	void Kill();

	Entity(const FString& name, const Entity& parent);
	Entity(GEntityID* eid, const FString& name);
	Entity(Entity&& other) noexcept;
	Entity(const Entity& other);
	Entity& operator=(Entity&& other) noexcept;
	void operator=(const Entity& other);

	~Entity();
	Entity();

	TransformMode TransformMode;

private:

	friend class EntityManager;
	friend class FEntityMemoryStack;
	friend class Scene;

	FSharedPtr<GEntityID> EntityId;
	FString Name;
};