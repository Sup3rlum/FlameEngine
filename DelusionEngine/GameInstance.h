#pragma once

#include "nchfx.h"

#include "ContextDescription.h"
#include "GLFWContext.h"
#include "Context.h"
#include "Scene.h"
#include "Version.h"


EXPORT_CLASS GameInstance
{
public:
	GameInstance(STRING NAME, DVERSION VER);
	~GameInstance();

	DRESULT Attach(Context* _context);
	DRESULT Dettach();
	DRESULT Start();

	Context* _currentContext;

	STRING Name;
	DVERSION Version;
	bool IsAttached;

	Scene* _currentScene;
};

