
#include "FRICommandList.h"

FRICommandListExecutor GCommandExecutor;


void FRICommandList::Flush()
{
	GCommandExecutor.Execute(*this);
}

