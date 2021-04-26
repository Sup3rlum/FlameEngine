#pragma once


#include "EntityComponent.h"
#include "EntityWorld.h"



template<typename...TComponents>
struct FEntityComponentSystem
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

				//std::cout << "Indexing Component " << typeid(T).name() << " at index: " << i << std::endl;

			}
		}
	}

	FORCEINLINE void IndexStack(FEntityMemoryStack* stack)
	{

		/*std::cout << std::endl;
		std::cout << "Starting index, with types: " << std::endl;

		for (int i = 0; i < stack->BlockArchetype.NumComponentTypes; i++)
		{

			std::cout << "Stack Component ("<< stack->BlockArchetype.ComponentTypes[i]._TypeId <<"): " << stack->BlockArchetype.ComponentTypes[i]._DecoratedName << " at index: " << i << std::endl;
		}

		std::cout << std::endl;*/

		(SetComponentTypeIndex<TComponents>(stack), ...);
	}


protected:
	FEntityComponentSystem(EntityWorld* entityWorld) :
		systemArchetype(TEntityArchetype<TComponents...>()),
		entityWorld(entityWorld)
	{
	}

	FEntityComponentSystem() :
		entityWorld(NULL),
		systemArchetype(TEntityArchetype<TComponents...>())
	{

	}

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


	// I like how fast it currently is, however im not sure how stable/scalable the indexing magic is
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

	FEntityArchetype systemArchetype;
	EntityWorld* entityWorld;


	friend class Scene;

};