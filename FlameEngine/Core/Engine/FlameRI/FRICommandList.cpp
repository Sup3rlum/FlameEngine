
#include "FRICommandList.h"

FRICommandListExecutor GCommandExecutor;


void FRICommandListBase::Flush()
{
	GCommandExecutor.Execute(*this);
}

