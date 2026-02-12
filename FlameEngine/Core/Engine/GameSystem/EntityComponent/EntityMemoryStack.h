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


EXPORT(struct, FEntityMemoryStack)
{

	EXPORT(struct, FMemBlock)
	{
		FEntityMemoryStack* Parent;
		FMemBlock* Next;
		uint64 ColumnSize;
		uint64 Columns;
		uint64 Rows;
		uint64 NumEntities;

		Entity* controlArray;

		byte* Data;

		void SetDataPtr(uint64 Alignment);
		byte* GetColumn(uint64 column) const;
		uint64 GetSize() const;
		byte* GetMemPtr(uint64 column, uint32 row) const;

		template<typename TComponent>
		FORCEINLINE TComponent* GetComponentPtr(uint64 column, int32 row)
		{
			return (TComponent*)GetMemPtr(column, row);
		}

		template<typename TComponent>
		FORCEINLINE TComponent* GetComponentPtr(uint64 column, int32 row) const
		{
			return (TComponent*)GetMemPtr(column, row);
		}

		template<typename TComponent>
		FORCEINLINE TComponent* GetComponentPtr(uint64 column)
		{
			int32 row = Parent->BlockArchetype.GetIndex<TComponent>();
			return GetComponentPtr<TComponent>(column, row);
		}

		template<typename TComponent>
		FORCEINLINE const TComponent* GetComponentPtr(uint64 column) const
		{
			int32 row = Parent->BlockArchetype.GetIndex<TComponent>();
			return GetComponentPtr<TComponent>(column, row);
		}




		template<typename TComponent>
		FORCEINLINE TComponent& GetComponent(uint64 column, int32 row)
		{
			return *GetComponentPtr<TComponent>(column, row);
		}


		template<typename TComponent>
		FORCEINLINE const TComponent& GetComponent(uint64 column, int32 row) const
		{
			return *GetComponentPtr<TComponent>(column, row);
		}


		template<typename TComponent>
		FORCEINLINE TComponent& GetComponent(uint64 column)
		{
			int32 row = Parent->BlockArchetype.GetIndex<TComponent>();
			return GetComponent<TComponent>(column, row);
		}

		template<typename TComponent>
		FORCEINLINE const TComponent& GetComponent(uint64 column) const
		{
			int32 row = Parent->BlockArchetype.GetIndex<TComponent>();
			return GetComponent<TComponent>(column, row);
		}

		FORCEINLINE bool IsFull() const
		{
			return NumEntities == Columns;
		}
	};

	FEntityMemoryStack(FEntityArchetype archetype, uint32 initialCapacity);
	Entity AllocEntity(const FString&);
	void AllocBlock(uint32 blockCapacity);
	void EnsureCapacity();
	void FreeEntity(GEntityID entity);
	void Flush();
	//void AllocEntityMemory();
	void CleanBlocks();
	~FEntityMemoryStack();


	FEntityArchetype BlockArchetype;
	FMemBlock* Top;
	uint64* Offsets;

};