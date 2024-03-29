#include "Entity.h"


FEntityMemoryStack::FEntityMemoryStack(FEntityArchetype archetype, uint32 initialCapacity) :
	BlockArchetype(archetype),
	Top(NULL)
{
	AllocBlock(initialCapacity);

	Offsets = new uint64[archetype.NumComponentTypes];
	uint64 total = 0;

	for (int i = 0; i < archetype.NumComponentTypes; i++)
	{
		Offsets[i] = total;
		total += archetype.ComponentTypes[i].SizeOf();
	}

}


Entity FEntityMemoryStack::AllocEntity(const FString& name)
{
	if (Top->IsFull())
	{
		AllocBlock(FEntityMemoryAllocator::BlockCapacityDefault);
	}

	auto newEntId = new GEntityID
	{ 
		Top, 
		Top->NumEntities 
	};

	Top->NumEntities++;

	new (&newEntId->GetControl()) Entity(newEntId, name);
	return newEntId->GetControl();
}


void FEntityMemoryStack::AllocEntityMemory()
{
	if (Top->IsFull())
	{
		AllocBlock(FEntityMemoryAllocator::BlockCapacityDefault);
	}
	Top->NumEntities++;
}


void FEntityMemoryStack::FreeEntity(GEntityID entityId)
{
	memcpy(
		entityId.Block->GetColumn(entityId.Index), 
		Top->GetColumn(--(Top->NumEntities)),
		BlockArchetype.MemColumnSize
	);

	//entityId.GetControl().Invalidate();
	entityId.GetControl() = Top->controlArray[Top->NumEntities];
	*entityId.GetControl().EntityId = entityId;
}


void FEntityMemoryStack::AllocBlock(uint32 blockCapacity)
{

	FEntityMemoryBlock* currentTop = Top;

	uint64 Alignment = BlockArchetype.MemAlignment;
	uint64 allocSize = sizeof(FEntityMemoryBlock) + blockCapacity * BlockArchetype.MemColumnSize + Alignment;

	Top = Memory::Alloc<FEntityMemoryBlock>(allocSize);
	Memory::Zero(Top, allocSize);

	Top->ColumnSize = BlockArchetype.MemColumnSize;
	Top->Columns = blockCapacity;
	Top->Rows = BlockArchetype.NumComponentTypes;
	Top->NumEntities = 0;
	Top->Parent = this;
	Top->Next = currentTop;
	Top->SetDataPtr(Alignment);
	Top->controlArray = new Entity[blockCapacity];

}


void FEntityMemoryStack::Flush()
{
	auto CurrentTop = Top;

	while (CurrentTop)
	{
		auto RemoveBlock = CurrentTop;
		CurrentTop = CurrentTop->Next;

		free(RemoveBlock);
	}
}
