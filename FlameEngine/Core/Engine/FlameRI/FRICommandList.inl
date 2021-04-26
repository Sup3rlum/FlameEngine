#pragma once



class FRICommandList;
class FRICommandBase;




class FRICommandListIterator
{
public:
	FRICommandListIterator(FRICommandListBase& CmdList)
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

struct FRICommandListExecutor
{
	void Execute(FRICommandListBase& cmdList)
	{
		FRICommandListIterator Iter(cmdList);

		while (Iter.HasCommandsLeft())
		{
			FRICommandBase* Cmd = Iter.NextCommand();
			CurrentCommand = Cmd;
			Cmd->ExecuteCmd(cmdList);
		}
	}

	FRICommandBase* CurrentCommand;
};