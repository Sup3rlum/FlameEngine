#pragma once


#include "EntityComponent.h"
#include "EntityWorld.h"



class Scene;

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
		for (auto& [archType, stack] : scene->entityWorld.EntMemory)
		{
			if (stack->BlockArchetype.Contains(systemArchetype))
			{
				IndexStack(stack);

				auto block = stack->Top;
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
	FEntityComponentSystem(Scene* scene) :
		systemArchetype(TEntityArchetype<TComponents...>()),
		scene(scene)
	{
	}

	FEntityComponentSystem() :
		systemArchetype(TEntityArchetype<TComponents...>()),
		scene(NULL)
	{
	}

	virtual void Update(Entity ent, TComponents&...)
	{
	}

	Scene* scene;

public:

	FEntityComponentSystem(FEntityComponentSystem&& other) noexcept :
		systemArchetype(other.systemArchetype),
		scene(other.scene)
	{
	}

	FEntityComponentSystem(const FEntityComponentSystem& other) noexcept :
		systemArchetype(other.systemArchetype),
		scene(other.scene)
	{
	}

	FEntityComponentSystem& operator=(const FEntityComponentSystem& other)
	{
		systemArchetype = other.systemArchetype;
		scene = other.scene;

		return *this;
	}

	template<typename TLambda>
	void ForEach(TLambda lambdaFunc)
	{
		for (auto& [archType, stack] : scene->entityWorld.EntMemory)
		{
			if (stack->BlockArchetype.Contains(systemArchetype))
			{
				IndexStack(stack);

				auto block = stack->Top;
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

		for (auto& [archType, stack] : scene->entityWorld.EntMemory)
		{
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
	friend class Scene;
};