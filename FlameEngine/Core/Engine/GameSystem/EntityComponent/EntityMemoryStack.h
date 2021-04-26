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



struct GEntityId;
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
		FORCEINLINE TComponent& GetComponent(uint64 column, uint64 row)
		{
			//std::cout << "Retrieving Component " << typeid(TComponent).name() << " at index: " << row << std::endl;


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


	FEntityMemoryStack(FEntityArchetype archetype, uint32 initialCapacity) :
		BlockArchetype(archetype),
		Top(NULL)
	{
		AllocBlock(initialCapacity);

		Offsets = new uint64[archetype.NumComponentTypes];
		uint64 total = 0;

		for (int i = 0; i < archetype.NumComponentTypes; i++)
		{
			Offsets[i] = total;
			total += archetype.ComponentTypes[i]._Size;
		}

	}

	Entity AllocEntity(const FString&);
	void FreeEntity(GEntityId);
	void AllocBlock(uint32 blockCapacity);

	void Flush()
	{
		auto CurrentTop = Top;

		while (CurrentTop)
		{
			auto RemoveBlock = CurrentTop;
			CurrentTop = CurrentTop->Next;

			//std::cout << "Removing block: " << RemoveBlock << std::endl;
			free(RemoveBlock);
		}
	}




	~FEntityMemoryStack()
	{
		Flush();

		delete Offsets;
	}

};