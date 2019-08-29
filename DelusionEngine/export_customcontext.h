#pragma once
/*
#include "ContextManager.h"

EXTERN_C_START

DELUSION_DLL void Delusion_ContextManager_AttachToHandle(HWND _hwnd, unsigned int width, unsigned int height)
{
	ContextDescription _contextDesc;

	_contextDesc.contextSampleCount = 4;
	_contextDesc.contextMajorVersion = 4;
	_contextDesc.contextMinorVersion = 5;
	_contextDesc.contextProfile = ContextProfile::OPENGL_CORE;
	_contextDesc.contextCompatibility = 1;
	_contextDesc.width = width;
	_contextDesc.height = height;
	_contextDesc.fullscreen = true;


	ContextManager::AttachToHandle(_hwnd, _contextDesc);
	ContextManager::Initialize();
}

DELUSION_DLL void Delusion_ContextManager_SwapBuffers()
{
	ContextManager::_currentContext->SwapChain();
}

DELUSION_DLL void Delusion_ContextManager_Clear()
{
	ContextManager::ManualLoop();
}


EXTERN_C_END*/