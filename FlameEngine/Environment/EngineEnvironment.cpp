#include "EngineEnvironment.h"

#include <windows.h>

FString FEngineEnvironment::GetInstancePath()
{
	FStaticString<512> szBuffer;
	GetModuleFileName(NULL, szBuffer, 512);

	return szBuffer;
}