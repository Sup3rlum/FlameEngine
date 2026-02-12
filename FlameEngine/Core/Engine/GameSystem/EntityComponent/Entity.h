#pragma once


#include "EntityMemoryStack.h"
#include "Core/Math/Module.h"


struct EntityWorld;


struct GEntityID
{
	union
	{
		struct { FEntityMemoryStack::FMemBlock* Block; uint64 Index; };
		struct { uint64 High, Low; };
		struct { void* HighPtr, *LowPtr; };
	};

	class Entity GetControl();
	class Entity& GetControlRef();
	bool IsValid();
};


enum class TransformMode
{
	Absolute,
	Relative
};
/*  TODO:
struct EntityDetail
{
	FString Name;
	GEntityID EntityId;
	GEntityID ParentId;
	FArray<struct Entity> _Children;
	::TransformMode _TransformMode;
};*/

EXPORT(struct, Entity)
{

private:
	FORCEINLINE const FEntityMemoryStack::FMemBlock* MemBlock() const
	{
		return EntityId->Block;
	}

	FORCEINLINE FEntityMemoryStack::FMemBlock* MemBlock()
	{
		return EntityId->Block;
	}

public:

	template<typename TComponent>
	void SetComponent(const TComponent& component)
	{
		Memory::Copy(MemBlock()->GetComponentPtr<TComponent>(EntityId->Index), &component, sizeof(TComponent));
	}

	template<typename TComponent>
	void InitComponent()
	{
		new (MemBlock()->GetComponentPtr<TComponent>(EntityId->Index)) TComponent;
	}

	template<typename TComponent>
	TComponent& Component()
	{
		return MemBlock()->GetComponent<TComponent>(EntityId->Index);
	}


	template<typename TComponent>
	const TComponent& Component() const
	{
		return MemBlock()->GetComponent<TComponent>(EntityId->Index);
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


	void AddChild(Entity Child);

	FTransform& Transform();
	const FTransform& Transform() const;
	FTransform GlobalTransform() const;
	TransformMode& TransformMode();
	const ::TransformMode& TransformMode() const;
	const FArray<struct Entity>& Children() const;
	FArray<struct Entity>& Children();

	Entity GetParent() const;
	uint32 GetRefCount() const;
	const FEntityArchetype& GetArchetype() const;

	FString GetName() const;
	bool IsValid() const;

	void Kill();

	Entity(const FString& name, const Entity& parent);
	Entity(GEntityID* eid, const FString& name);
	Entity(Entity&& other) noexcept;
	Entity(const Entity& other);
	Entity& operator=(Entity&& other) noexcept;
	Entity& operator=(const Entity& other);

	~Entity();
	Entity();

private:

	friend class EntityManager;
	friend class FEntityMemoryStack;
	friend class Scene;

	FSharedPtr<::TransformMode> _TransformMode;
	FSharedPtr<FArray<struct Entity>> _Children;
	FSharedPtr<GEntityID> EntityId;
	FSharedPtr<GEntityID> ParentId;
	FSharedPtr<FString> Name;
};