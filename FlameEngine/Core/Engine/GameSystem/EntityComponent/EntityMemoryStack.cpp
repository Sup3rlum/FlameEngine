#include "Entity.h"


FEntityMemoryStack::FEntityMemoryStack(FEntityArchetype archetype, uint32 initialCapacity) :
	BlockArchetype(archetype),
	Top(NULL)
{
	AllocBlock(initialCapacity);

	Offsets = new uint64[archetype.ComponentTypes.Length()];
	uint64 total = 0;

	for (int i = 0; i < archetype.ComponentTypes.Length(); i++)
	{
		Offsets[i] = total;
		total += archetype.ComponentTypes[i].Key.SizeOf();
	}

}

void FEntityMemoryStack::EnsureCapacity()
{
	if (Top->IsFull())
	{
		AllocBlock(FEntityMemoryAllocator::BlockCapacityDefault);
	}

}

Entity FEntityMemoryStack::AllocEntity(const FString& name)
{
	EnsureCapacity();

	auto newEntId = new GEntityID
	{ 
		Top, 
		Top->NumEntities 
	};

	Top->NumEntities++;

	new (&newEntId->GetControlRef()) Entity(newEntId, name);
	return newEntId->GetControlRef();
}

void FEntityMemoryStack::AllocBlock(uint32 blockCapacity)
{
	FMemBlock* currentTop = Top;

	uint64 Alignment = BlockArchetype.MemAlignment;
	uint64 allocSize = sizeof(FMemBlock) + blockCapacity * BlockArchetype.MemColumnSize + Alignment;

	Top = Memory::Alloc<FMemBlock>(allocSize);
	Memory::Zero(Top, allocSize);

	Top->ColumnSize = BlockArchetype.MemColumnSize;
	Top->Columns = blockCapacity;
	Top->Rows = BlockArchetype.ComponentTypes.Length();
	Top->NumEntities = 0;
	Top->Parent = this;
	Top->Next = currentTop;
	Top->SetDataPtr(Alignment);
	Top->controlArray = new Entity[blockCapacity];
}

void FEntityMemoryStack::FreeEntity(GEntityID entityId)
{
	Top->NumEntities--;


	// TODO:
	// Add deallocation func for all components

	for (int i = 0; i < BlockArchetype.ComponentTypes.Length(); i++)
	{
		auto RemoveComp = entityId.Block->GetMemPtr(entityId.Index, i);
		BlockArchetype.ComponentTypes[i].Value(RemoveComp);
	}

	// Move top entity to fill in gap, so that block is continuous
	Memory::Copy(
		entityId.Block->GetColumn(entityId.Index),
		Top->GetColumn(Top->NumEntities),
		BlockArchetype.MemColumnSize
	);

	//entityId.GetControl().Invalidate();
	entityId.GetControl() = Top->controlArray[Top->NumEntities];
	*entityId.GetControl().EntityId = entityId;

	CleanBlocks();
}

// Removes All trailing blocks that are empty
void FEntityMemoryStack::CleanBlocks()
{
	if (Top && Top->Next && Top->NumEntities == 0)
	{
		auto RemoveBlock = Top;
		Top = Top->Next;
		free(RemoveBlock);
	}
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

FEntityMemoryStack::~FEntityMemoryStack()
{
	Flush();
	delete Offsets;
}


void FEntityMemoryStack::FMemBlock::SetDataPtr(uint64 Alignment)
{
	Data = Align(((byte*)this) + sizeof(FMemBlock), Alignment);
}

byte* FEntityMemoryStack::FMemBlock::GetColumn(uint64 column) const
{
	return Data + column * ColumnSize;
}

uint64 FEntityMemoryStack::FMemBlock::GetSize() const
{
	return sizeof(FMemBlock) + ColumnSize * Columns;
}

byte* FEntityMemoryStack::FMemBlock::GetMemPtr(uint64 column, uint32 row) const
{
	return GetColumn(column) + Parent->Offsets[row];
}
