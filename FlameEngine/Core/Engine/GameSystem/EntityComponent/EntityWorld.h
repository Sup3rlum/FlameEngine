#pragma once

#include "EntityMemoryStack.h"
#include "Core/Framework/Common/FHashMap.h"

struct FEntityArchetypeHasher
{
	size_t operator ()(const FEntityArchetype& archetype) const
	{
		return FHashFuncImpl::MurmurHash3_x86_32(archetype.ComponentTypes, sizeof(FComponentType) * archetype.NumComponentTypes, 100);
	}
};


class Entity;

EXPORT(class, EntityWorld)
{
public:

	void AllocArchetype(const FEntityArchetype & archetype);
	Entity CreateEntityFromArchetype(const FString& name, const FEntityArchetype& archetype);
	~EntityWorld();

	FHashMap<FEntityArchetype, FEntityMemoryStack*, FEntityArchetypeHasher> EntMemory;

	friend class Entity;
};