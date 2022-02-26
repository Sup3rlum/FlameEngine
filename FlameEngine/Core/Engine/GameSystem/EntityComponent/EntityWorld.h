#pragma once

#include "EntityMemoryStack.h"
#include "Core/Framework/Common/FHashMap.h"

struct FEntityArchetypeHasher
{
	size_t operator ()(const FEntityArchetype& archetype) const
	{
		return archetype.HashCode;
	}
};


class Entity;

EXPORT(class, EntityWorld)
{
public:

	void AllocArchetype(const FEntityArchetype & archetype);
	Entity CreateEntityFromArchetype(const FString& name, const FEntityArchetype& archetype);
	~EntityWorld();


	void CopyEntMemory();

	FHashMap<FEntityArchetype, FEntityMemoryStack*, FEntityArchetypeHasher> StageMemory;
	FHashMap<FEntityArchetype, FEntityMemoryStack*, FEntityArchetypeHasher> EntMemory;


	friend class Entity;
};