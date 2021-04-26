
#include "EntityWorld.h"
#include "Entity.h"

Entity EntityWorld::CreateEntityFromArchetype(const FString& name, const FEntityArchetype& archetype)
{
	AllocArchetype(archetype);
	return EntMemory.Get(archetype)->AllocEntity(name);
}