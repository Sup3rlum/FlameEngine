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



	void AllocArchetype(const FEntityArchetype& archetype)
	{
		if (!EntMemory.Contains(archetype))
		{
			EntMemory.Set(archetype, new FEntityMemoryStack(archetype, FEntityMemoryAllocator::BlockCapacityDefault));
		}
	}


	Entity CreateEntityFromArchetype(const FString& name, const FEntityArchetype& archetype);

	~EntityWorld()
	{
		auto start = EntMemory.Begin();
		auto end = EntMemory.End();

		for (auto it = start; it != end; it++)
		{
			delete it->Value;
		}

	}


	FHashMap<FEntityArchetype, FEntityMemoryStack*, FEntityArchetypeHasher> EntMemory;




	friend class Entity;

};