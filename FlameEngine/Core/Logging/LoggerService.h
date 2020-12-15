#pragma once

#include "Core/Common/CoreCommon.h"

EXPORT(enum class, LogMessage)
{
	MSG_ERROR,
	MSG_WARNING,
	MSG_INFO,
	MSG_NONE
};

EXPORT(class,  LoggerService)
{
public:
	FString data;

	void PushMessage(LogMessage type, FString msg);

	static LoggerService* pHandlingInstance;

};



#define FLAME_INFO(b) LoggerService::pHandlingInstance->PushMessage(LogMessage::MSG_INFO, b)
#define FLAME_ERROR(b) LoggerService::pHandlingInstance->PushMessage(LogMessage::MSG_ERROR, b)