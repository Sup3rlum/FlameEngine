#include "LoggerService.h"

LoggerService* LoggerService::pHandlingInstance = NULL;

void LoggerService::PushMessage(LogMessage type, FString msg)
{
	FString fMessagePrefix = "";

	switch (type)
	{
	case LogMessage::MSG_ERROR:
		fMessagePrefix = "[ERROR] ";
		break;
	case LogMessage::MSG_WARNING:
		fMessagePrefix = "[WARNING] ";
		break;
	case LogMessage::MSG_INFO:
		fMessagePrefix = "[INFO] ";
		break;
	case LogMessage::MSG_NONE:
		break;
	default:
		break;
	}

	data += (fMessagePrefix + msg + "\n");
}