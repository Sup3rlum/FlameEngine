#pragma once

#include "../dll/framework.h"
#include "../util/macros.h"


EXPORT_ENUM LogMessage
{
	MSG_ERROR,
	MSG_WARNING,
	MSG_INFO,
	MSG_NONE
};

EXPORT_CLASS LoggerService
{
public:
	STRING data;

	void PushMessage(LogMessage type, STRING msg);

	static LoggerService* pHandlingInstance;

};



#define FLAME_INFO(b) LoggerService::pHandlingInstance->PushMessage(LogMessage::MSG_INFO, b)
#define FLAME_ERROR(b) LoggerService::pHandlingInstance->PushMessage(LogMessage::MSG_ERROR, b)