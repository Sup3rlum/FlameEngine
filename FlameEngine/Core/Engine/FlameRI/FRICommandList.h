#pragma once

#include "FRICommand.h"
#include "FRIContext.h"
#include "Core/Runtime/Common/MemoryStack.h"



struct FRICommandList
{

	void Flush();


	FORCEINLINE void* AllocCommand(int32 AllocSize, int32 Alignment)
	{
		FRICommandBase* Result = (FRICommandBase*)MemoryStack.Alloc(AllocSize, Alignment);
		++NumCommands;
		*CommandLink = Result;
		CommandLink = &Result->Next;
		return Result;
	}
	template <typename TCmd>
	FORCEINLINE void* AllocCommand()
	{
		return AllocCommand(sizeof(TCmd), alignof(TCmd));
	}

	MemoryStack MemoryStack;
	FRICommandBase* First;
	FRICommandBase** CommandLink;
	uint32 NumCommands;
};


struct FRICommandListExecutor
{
	void Execute(FRICommandList cmdList)
	{
		FRICommandListIterator Iter(cmdList);

		while (Iter.HasCommandsLeft())
		{
			FRICommandBase* Cmd = Iter.NextCommand();
			CurrentCommand = Cmd;
			Cmd->ExecuteCmd();
		}
	}

	FRICommandBase* CurrentCommand;
};

#include "FRICommandList.inl"