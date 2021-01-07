#pragma once



class FRICommandList;
class FRICommandBase;




class FRICommandListIterator
{
public:
	FRICommandListIterator(FRICommandList& CmdList)
	{
		CmdPtr = CmdList.First;
		NumCommands = 0;
		CmdListNumCommands = CmdList.NumCommands;
	}
	~FRICommandListIterator()
	{
		//checkf(CmdListNumCommands == NumCommands, TEXT("Missed %d Commands!"), CmdListNumCommands - NumCommands);
	}

	FORCEINLINE bool HasCommandsLeft() const
	{
		return !!CmdPtr;
	}

	FORCEINLINE FRICommandBase* NextCommand()
	{
		FRICommandBase* RHICmd = CmdPtr;
		CmdPtr = RHICmd->Next;
		NumCommands++;
		return RHICmd;
	}

private:
	FRICommandBase* CmdPtr;
	uint32 NumCommands;
	uint32 CmdListNumCommands;
};