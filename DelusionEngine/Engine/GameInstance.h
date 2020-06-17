#pragma once

#include "../dll/nchfx.h"
#include "../dll/Version.h"

#include "../Context/ContextDescription.h"
#include "../Context/GLFWContext.h"
#include "../Context/Context.h"
#include "../Scene/Scene.h"

#include "../Graphics/RenderState.h"


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

