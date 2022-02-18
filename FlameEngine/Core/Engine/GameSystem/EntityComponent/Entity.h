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

	bool IsValid();
};


EXPORT(struct, Entity)
{

	template<typename TComponent>
	const int Find() const
	{
		FComponentType compareType = TComponentType<TComponent>();

		for (int row = 0; row < EntityId->Block->Parent->BlockArchetype.NumComponentTypes; row++)
		{
			if (compareType == EntityId->Block->Parent->BlockArchetype.ComponentTypes[row])
			{
				return row;
			}
		}

		return -1;
	}
	template<typename TComponent>
	int Find()
	{
		FComponentType compareType = TComponentType<TComponent>();

		for (int row = 0; row < EntityId->Block->Parent->BlockArchetype.NumComponentTypes; row++)
		{
			if (compareType == EntityId->Block->Parent->BlockArchetype.ComponentTypes[row])
			{
				return row;
			}
		}

		return -1;
	}

	template<typename TComponent>
	void SetComponent(const TComponent& component)
	{
		EntityId->Block->GetComponent<TComponent>(EntityId->Index, Find<TComponent>()) = component;
	}

	template<typename TComponent, typename...TCreationArgs>
	void InitComponent(const TCreationArgs& ... args)
	{
		new (&EntityId->Block->GetComponent<TComponent>(EntityId->Index, Find<TComponent>())) TComponent(args...);
	}


	template<typename TComponent>
	TComponent& Component()
	{
		return EntityId->Block->GetComponent<TComponent>(EntityId->Index, Find<TComponent>());
	}


	template<typename TComponent>
	const TComponent& Component() const
	{
		return EntityId->Block->GetComponent<TComponent>(EntityId->Index, Find<TComponent>());
	}

	template<typename TComponent>
	bool HasComponent() const
	{
		return Find<TComponent>() != -1;
	}

	template<typename TComponent>
	void AddComponent() 
	{
		if (!HasComponent<TComponent>())
		{
			
		}
	}

	FORCEINLINE FEntityArchetype GetArchetype() const;
	uint32 GetRefCount() const;

	FString Name() const;
	bool IsValid() const;

	Entity();
	Entity(GEntityID* eid, const FString& name);
	Entity(Entity&& other) noexcept;
	Entity(const Entity& other);
	Entity& operator=(Entity&& other) noexcept;
	void operator=(const Entity& other);

	~Entity();

private:
	void Invalidate();
	void Delete();

	friend class EntityManager;
	friend class FEntityMemoryStack;
	friend class Scene;

	GEntityID* EntityId;
	uint32* refCount;
	FString name;

};