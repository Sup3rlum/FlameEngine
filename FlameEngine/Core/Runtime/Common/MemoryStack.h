#pragma once

#include "Core/Common/CoreCommon.h"

EXPORT(class, MemoryStack)

{

public:

	MemoryStack(int32 InMinMarksToAlloc = 1)
		: Top(nullptr)
		, End(nullptr)
		, TopChunk(nullptr)
		, TopMark(nullptr)
		, NumMarks(0)
		, MinMarksToAlloc(InMinMarksToAlloc)
	{
	}

	~MemoryStack()
	{
		FreeChunks(nullptr);
	}

	FORCEINLINE byte* PushBytes(int32 AllocSize, int32 Alignment)
	{
		return (byte*)Alloc(AllocSize, max(AllocSize >= 16 ? (int32)16 : (int32)8, Alignment));
	}

	FORCEINLINE void* Alloc(int32 AllocSize, int32 Alignment)
	{
		// Debug checks.
		/*checkSlow(AllocSize >= 0);
		checkSlow((Alignment & (Alignment - 1)) == 0);
		checkSlow(Top <= End);
		checkSlow(NumMarks >= MinMarksToAlloc);*/


		// Try to get memory from the current chunk.
		byte* Result = Align(Top, Alignment);
		byte* NewTop = Result + AllocSize;

		// Make sure we didn't overflow.
		if (NewTop <= End)
		{
			Top = NewTop;
		}
		else
		{
			// We'd pass the end of the current chunk, so allocate a new one.
			AllocateNewChunk(AllocSize + Alignment);
			Result = Align(Top, Alignment);
			NewTop = Result + AllocSize;
			Top = NewTop;
		}
		return Result;
	}

	/** return true if this stack is empty. */
	FORCEINLINE bool IsEmpty() const
	{
		return TopChunk == nullptr;
	}

	FORCEINLINE void Flush()
	{
		FreeChunks(nullptr);
	}
	FORCEINLINE int32 GetNumMarks()
	{
		return NumMarks;
	}
	/** @return the number of bytes allocated for this FMemStack that are currently in use. */
	int32 GetByteCount() const;

	// Returns true if the pointer was allocated using this allocator
	bool ContainsPointer(const void* Pointer) const;



	// Types.
	struct FTaggedMemory
	{
		FTaggedMemory* Next;
		int32 DataSize;

		byte* Data() const
		{
			return ((byte*)this) + sizeof(FTaggedMemory);
		}
	};

private:

	/**
	 * Allocate a new chunk of memory of at least MinSize size,
	 * updates the memory stack's Chunks table and ActiveChunks counter.
	 */
	void AllocateNewChunk(int32 MinSize);

	/** Frees the chunks above the specified chunk on the stack. */
	void FreeChunks(FTaggedMemory* NewTopChunk);

	// Variables.
	byte* Top;				// Top of current chunk (Top<=End).
	byte* End;				// End of current chunk.
	FTaggedMemory* TopChunk;			// Only chunks 0..ActiveChunks-1 are valid.

	/** The top mark on the stack. */
	class FMemMark* TopMark;

	/** The number of marks on this stack. */
	int32 NumMarks;

	/** Used for a checkSlow. Most stacks require a mark to allocate. Command lists don't because they never mark, only flush*/
	int32 MinMarksToAlloc;
};

