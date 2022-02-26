#pragma once


#include "EntityComponent.h"
#include "Core/Common/Alignment.h"

class FEntityMemoryAllocator
{
public:
	enum
	{
		BlockCapacityDefault = 50
	};
};


struct GEntityID;
struct Entity;


struct FEntityMemoryStack
{

	struct FEntityMemoryBlock
	{
		FEntityMemoryStack* Parent;
		FEntityMemoryBlock* Next;
		uint64 ColumnSize;
		uint64 Columns;
		uint64 Rows;
		uint64 NumEntities;

		Entity* controlArray;

		byte* Data;

		FORCEINLINE void SetDataPtr(uint64 Alignment)
		{
			Data = Align(((byte*)this) + sizeof(FEntityMemoryBlock), Alignment);
		}

		FORCEINLINE byte* GetColumn(uint64 column) const
		{
			return Data + column * ColumnSize;
		}

		FORCEINLINE uint64 GetSize() const
		{
			return sizeof(FEntityMemoryBlock) + ColumnSize * Columns;
		}

		template<typename TComponent>
		FORCEINLINE TComponent& GetComponent(uint64 column)
		{
			int32 row = Parent->BlockArchetype.GetIndex<TComponent>();

			return *(TComponent*)(GetColumn(column) + Parent->Offsets[row]);
		}

		template<typename TComponent>
		FORCEINLINE TComponent& GetComponent(uint64 column, int32 row)
		{
			return *(TComponent*)(GetColumn(column) + Parent->Offsets[row]);
		}

		FORCEINLINE bool IsFull() const
		{
			return NumEntities == Columns;
		}
	};

	FEntityArchetype BlockArchetype;
	FEntityMemoryBlock* Top;
	uint64* Offsets;

	FEntityMemoryStack(FEntityArchetype archetype, uint32 initialCapacity);
	Entity AllocEntity(const FString&);
	void AllocBlock(uint32 blockCapacity);

	void FreeEntity(GEntityID entity);
	void Flush();

	~FEntityMemoryStack()
	{
		Flush();
		delete Offsets;
	}

	void AllocEntityMemory();
};