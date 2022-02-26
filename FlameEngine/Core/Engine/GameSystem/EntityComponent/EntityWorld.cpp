
#include "EntityWorld.h"
#include "Entity.h"

#include <thread>

Entity EntityWorld::CreateEntityFromArchetype(const FString& name, const FEntityArchetype& archetype)
{
	AllocArchetype(archetype);

	StageMemory.Get(archetype)->AllocEntityMemory();

	return EntMemory.Get(archetype)->AllocEntity(name);
}

void EntityWorld::AllocArchetype(const FEntityArchetype& archetype)
{
	if (!EntMemory.Contains(archetype))
	{
		EntMemory.Set(archetype, new FEntityMemoryStack(archetype, FEntityMemoryAllocator::BlockCapacityDefault));
	}
	if (!StageMemory.Contains(archetype))
	{
		StageMemory.Set(archetype, new FEntityMemoryStack(archetype, FEntityMemoryAllocator::BlockCapacityDefault));
	}
}


void EntityWorld::CopyEntMemory()
{
	for (auto& [archType, stack] : EntMemory)
	{
		auto stackTop = stack->Top;
		auto stageStackTop = StageMemory.Get(archType)->Top;

		while (stackTop)
		{
			Memory::Copy(stageStackTop->Data, stackTop->Data, stackTop->GetSize() - sizeof(FEntityMemoryStack::FEntityMemoryBlock));
			Memory::Copy(stageStackTop->controlArray, stackTop->controlArray, sizeof(Entity) * stackTop->Columns);

			stackTop = stackTop->Next;
			stageStackTop = stageStackTop->Next;
		}
	}
}

EntityWorld::~EntityWorld()
{
	for (auto& [archType, stack] : EntMemory)
	{
		delete stack;
	}

	for (auto& [archType, stack] : StageMemory)
	{
		delete stack;
	}
}
