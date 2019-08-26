#pragma once



#include "nchfx.h"

#include "FrameTime.h"
#include "Scene.h"
#include "GLFWContext.h"
#include "CustomContext.h"

using namespace glm;

typedef void (*RenderCallback)(FrameTime*);


EXPORT_CLASS ContextManager
{
public:
	static FrameTime* _frameTime;
	static Scene* _scene;

	static void CreateNew(ContextDescription _description);
	static void AttachToHandle(HWND _hWnd);

	static void Initialize();

	static BaseContext* _currentContext;
	static RenderCallback _renderCallback;
};

