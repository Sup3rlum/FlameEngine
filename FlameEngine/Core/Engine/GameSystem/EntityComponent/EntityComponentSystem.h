#pragma once


#include "EntityComponent.h"
#include "EntityWorld.h"




struct FEntityComponentSystemBase
{
	virtual void Tick() = 0;
};


template<typename...TComponents>
struct FEntityComponentSystem : FEntityComponentSystemBase
{
private:

	// I am not completely sure how this works right now, but it does the job.
	template<typename TComp>
	struct TComponentIndexer
	{
		inline static int index = -1;
	};


	template<typename T>
	FORCEINLINE void SetComponentTypeIndex(FEntityMemoryStack* stack)
	{
		FComponentType cmp = TComponentType<T>();

		for (int i = 0; i < stack->BlockArchetype.NumComponentTypes; i++)
		{
			if (cmp == stack->BlockArchetype.ComponentTypes[i])
			{
				TComponentIndexer<T>::index = i;

			}
		}
	}

	FORCEINLINE void IndexStack(FEntityMemoryStack* stack)
	{
		(SetComponentTypeIndex<TComponents>(stack), ...);
	}

	void Tick()
	{
		auto begin = entityWorld->EntMemory.Begin();
		auto end = entityWorld->EntMemory.End();

		for (auto it = begin; it != end; it++)
		{
			auto stack = it->Value;

			if (stack->BlockArchetype.Contains(systemArchetype))
			{
				auto block = stack->Top;

				IndexStack(stack);

				while (block)
				{
					for (int i = 0; i < block->NumEntities; i++)
					{
						this->Update(block->controlArray[i], block->GetComponent<TComponents>(i, TComponentIndexer<TComponents>::index) ...);
					}

					block = block->Next;
				}
			}
		}
	}


protected:
	FEntityComponentSystem(EntityWorld* entityWorld) :
		systemArchetype(TEntityArchetype<TComponents...>()),
		entityWorld(entityWorld),
		scene(NULL)
	{
	}

	FEntityComponentSystem() :
		systemArchetype(TEntityArchetype<TComponents...>()),
		entityWorld(NULL),
		scene(NULL)
	{

	}

	virtual void Update(Entity ent, TComponents&...) = 0;

	class Scene* scene;

public:

	FEntityComponentSystem(FEntityComponentSystem&& other) noexcept :
		systemArchetype(other.systemArchetype),
		entityWorld(other.entityWorld)
	{
	}

	FEntityComponentSystem(const FEntityComponentSystem& other) noexcept :
		systemArchetype(other.systemArchetype),
		entityWorld(other.entityWorld)
	{
	}

	FEntityComponentSystem& operator=(const FEntityComponentSystem& other)
	{
		systemArchetype = other.systemArchetype;
		entityWorld = other.entityWorld;

		return *this;
	}

	template<typename TLambda>
	void ForEach(TLambda lambdaFunc)
	{

		auto begin = entityWorld->EntMemory.Begin();
		auto end = entityWorld->EntMemory.End();


		for (auto it = begin; it != end; it++)
		{
			auto stack = it->Value;

			if (stack->BlockArchetype.Contains(systemArchetype))
			{
				auto block = stack->Top;

				IndexStack(stack);

				while (block)
				{
					for (int i = 0; i < block->NumEntities; i++)
					{
						lambdaFunc(block->controlArray[i], block->GetComponent<TComponents>(i, TComponentIndexer<TComponents>::index) ...);
					}

					block = block->Next;
				}
			}

		}

	}

	int32 Count()
	{
		int32 entityCount = 0;

		auto begin = entityWorld->EntMemory.Begin();
		auto end = entityWorld->EntMemory.End();


		for (auto it = begin; it != end; it++)
		{
			auto stack = it->Value;

			if (stack->BlockArchetype.Contains(systemArchetype))
			{
				auto block = stack->Top;
				while (block)
				{
					entityCount += block->NumEntities;

					block = block->Next;
				}
			}

		}

		return entityCount;
	}

	FEntityArchetype systemArchetype;
	EntityWorld* entityWorld;


	friend class Scene;

};