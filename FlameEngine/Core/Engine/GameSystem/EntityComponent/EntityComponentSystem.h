#pragma once


#include "EntityComponent.h"
#include "EntityWorld.h"
#include "Entity.h"

#include <thread>
#include <algorithm>
#include <execution>


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

		for (int i = 0; i < stack->BlockArchetype.ComponentTypes.Length(); i++)
		{
			if (cmp == stack->BlockArchetype.ComponentTypes[i].Key)
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
		for (auto& [archType, stack] : scene->EntWorld.EntMemory)
		{
			if (!stack->BlockArchetype.Contains(Archetype))
				continue;

			IndexStack(stack);

			auto block = stack->Top;
			while (block)
			{
				for (int idx = 0; idx < block->NumEntities; idx++)
				{
					this->Update(block->controlArray[idx], block->GetComponent<TComponents>(idx, TComponentIndexer<TComponents>::index) ...);
				}
				block = block->Next;
			}	
		}
	}


protected:
	FEntityComponentSystem(Scene* scene) :
		Archetype(TEntityArchetype<TComponents...>()),
		scene(scene)
	{
	}

	FEntityComponentSystem() :
		Archetype(TEntityArchetype<TComponents...>()),
		scene(NULL)
	{
	}

	virtual void Update(Entity ent, TComponents&...)
	{
	}

	Scene* scene;

public:

	FEntityComponentSystem(FEntityComponentSystem&& other) noexcept :
		Archetype(other.Archetype),
		scene(other.scene)
	{
	}

	FEntityComponentSystem(const FEntityComponentSystem& other) noexcept :
		Archetype(other.Archetype),
		scene(other.scene)
	{
	}

	FEntityComponentSystem& operator=(const FEntityComponentSystem& other)
	{
		Archetype = other.Archetype;
		scene = other.scene;

		return *this;
	}

	template<typename TLambda>
	int32 ForEach(TLambda lambdaFunc)
	{

		int32 IterCount = 0;
		for (auto& [archType, stack] : scene->EntWorld.EntMemory)
		{
			if (!stack->BlockArchetype.Contains(Archetype))
				continue;

			IndexStack(stack);

			auto block = stack->Top;
			while (block)
			{
				for (int idx = 0; idx < block->NumEntities; idx++)
				{
					lambdaFunc(block->controlArray[idx], block->GetComponent<TComponents>(idx, TComponentIndexer<TComponents>::index) ...);
				}
				IterCount += block->NumEntities;
				block = block->Next;
			}
		}

		return IterCount;
	}

	template<typename TLambda>
	int32 ParallelForEach(TLambda lambdaFunc)
	{
		int32 IterCount = 0;
		for (auto& [archType, stack] : scene->EntWorld.EntMemory)
		{
			if (!stack->BlockArchetype.Contains(Archetype))
				continue;

			IndexStack(stack);

			auto block = stack->Top;
			while (block)
			{
				FRange range(0, block->NumEntities);

				std::for_each(std::execution::par_unseq,
					range.begin(),
					range.end(),
					[&](int64& idx)
					{

						lambdaFunc(block->controlArray[idx], block->GetComponent<TComponents>(idx, TComponentIndexer<TComponents>::index) ...);
					});

				block = block->Next;
			}
		}
		
		return IterCount;
	}


	Entity First()
	{
		for (auto& [archType, stack] : scene->EntWorld.EntMemory)
		{
			if (!stack->BlockArchetype.Contains(Archetype))
				continue;

			IndexStack(stack);

			auto block = stack->Top;

			if (block)
			{
				if (block->NumEntities)
					return block->controlArray[0];
			}
		}

		return Entity();
	}


	int32 Count()
	{
		int32 entityCount = 0;

		for (auto& [archType, stack] : scene->EntWorld.EntMemory)
		{
			if (stack->BlockArchetype.Contains(Archetype))
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


	FEntityArchetype Archetype;
	friend class Scene;
};