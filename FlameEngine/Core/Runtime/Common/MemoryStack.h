#pragma once

#include "Core/Common/Alignment.h"
#include "Memory.h"




class FMemoryBlockAllocator
{
public:
	enum
	{
		PageBlockSize = 6144
	};
};


class FMemoryStack
{

public:

	struct FMemoryBlockHeader
	{
		FMemoryBlockHeader* Next;
		int32 DataSize;

		byte* Data() const
		{
			return ((byte*)this) + sizeof(FMemoryBlockHeader);
		}
		int32 BlockSize() const
		{
			return DataSize + sizeof(FMemoryBlockHeader);
		}
	};

	FMemoryStack() :
		Top(nullptr),
		End(nullptr),
		TopBlock(nullptr)
	{

	}

	void* Alloc(int32 AllocSize, int32 Alignment)
	{
		byte* AlignedTop = Align(Top, Alignment);
		byte* NewTop = AlignedTop + AllocSize;

		// Check if there is still memory left in the current memory block
		if (NewTop <= End)
		{
			// Move the top pointer to the new top of the current memory block
			Top = NewTop;
		}
		// There is not enough memory in the current memory block, so allocate a new one:
		else
		{
			AllocateNewBlock(AllocSize + Alignment);
			AlignedTop = Align(Top, Alignment);
			Top = AlignedTop + AllocSize;
		}

		return AlignedTop;
	}

	FORCEINLINE void AllocateNewBlock(int32 MinMemorySize)
	{
		FMemoryBlockHeader* Block = nullptr;

		// Get the minimum space required for the chunk
		uint32 TotalSize = MinMemorySize + sizeof(FMemoryBlockHeader);

		uint32 AllocSize = AlignNonPowerOf2(TotalSize, FMemoryBlockAllocator::PageBlockSize);
		Block = (FMemoryBlockHeader*)malloc(AllocSize);

		Block->DataSize = AllocSize - sizeof(FMemoryBlockHeader);
		Block->Next = TopBlock;
		TopBlock = Block;

		Top = Block->Data();
		End = Top + Block->DataSize;


	}

	FORCEINLINE void FreeBlocks(FMemoryBlockHeader* NewTopBlock)
	{
		while (TopBlock != NewTopBlock)
		{
			FMemoryBlockHeader* RemoveBlock = TopBlock;
			TopBlock = TopBlock->Next;

			free(RemoveBlock);
		}

		Top = nullptr;
		End = nullptr;

		if (TopBlock)
		{
			Top = TopBlock->Data();
			End = Top + TopBlock->DataSize;
		}

	}


	FORCEINLINE int32 GetByteCount() const
	{
		int32 Count = 0;
		for (FMemoryBlockHeader* Block = TopBlock; Block; Block = Block->Next)
		{
			if (Block != TopBlock)
			{
				Count += Block->DataSize;
			}
			else
			{
				Count += Top - Block->Data();
			}
		}
		return Count;
	}

	FORCEINLINE void PopBlock()
	{
		if (TopBlock)
		{
			FreeBlocks(TopBlock->Next);
		}
	}


	void Flush()
	{
		FreeBlocks(nullptr);
	}

	~FMemoryStack()
	{
		Flush();
	}

	byte* Top;
	byte* End;
	FMemoryBlockHeader* TopBlock;

};

