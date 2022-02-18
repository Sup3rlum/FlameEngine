
#include "EntityWorld.h"
#include "Entity.h"

Entity EntityWorld::CreateEntityFromArchetype(const FString& name, const FEntityArchetype& archetype)
{
	AllocArchetype(archetype);
	return EntMemory.Get(archetype)->AllocEntity(name);
}

void EntityWorld::AllocArchetype(const FEntityArchetype& archetype)
{
	if (!EntMemory.Contains(archetype))
	{
		EntMemory.Set(archetype, new FEntityMemoryStack(archetype, FEntityMemoryAllocator::BlockCapacityDefault));
	}
}

EntityWorld::~EntityWorld()
{
	for (auto& [archType, stack] : EntMemory)
	{
		delete stack;
	}
}
