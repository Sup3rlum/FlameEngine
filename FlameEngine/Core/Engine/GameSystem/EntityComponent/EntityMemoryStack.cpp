#include "Entity.h"


Entity FEntityMemoryStack::AllocEntity(const FString& name)
{

	if (Top->IsFull())
	{
		AllocBlock(FEntityMemoryAllocator::BlockCapacityDefault);
	}


	new (&Top->controlArray[Top->NumEntities]) Entity(new GEntityId{ Top, Top->NumEntities }, name);


	return Top->controlArray[Top->NumEntities++];
}

void FEntityMemoryStack::FreeEntity(GEntityId entityId)
{
	memcpy(entityId.Block->GetColumn(entityId.Index), Top->GetColumn(--(Top->NumEntities)), BlockArchetype.MemColumnSize);

	entityId.Block->controlArray[entityId.Index].Invalidate();
	entityId.Block->controlArray[entityId.Index] = Top->controlArray[Top->NumEntities];

	*entityId.Block->controlArray[entityId.Index].EntityId = entityId;
}


void FEntityMemoryStack::AllocBlock(uint32 blockCapacity)
{

	FEntityMemoryBlock* currentTop = Top;

	uint64 Alignment = BlockArchetype.MemAlignment;
	uint64 allocSize = sizeof(FEntityMemoryBlock) + blockCapacity * BlockArchetype.MemColumnSize + Alignment;

	Top = Memory::Alloc<FEntityMemoryBlock>(allocSize);

	Top->ColumnSize = BlockArchetype.MemColumnSize;
	Top->Columns = blockCapacity;
	Top->Rows = BlockArchetype.NumComponentTypes;
	Top->NumEntities = 0;
	Top->Parent = this;
	Top->Next = currentTop;
	Top->SetDataPtr(Alignment);
	Top->controlArray = new Entity[blockCapacity];

}
