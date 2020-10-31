#pragma once

#include "../dll/nchfx.h"
#include "../dll/Version.h"

#include "../Context/ContextDescription.h"
#include "../Context/GLFWContext.h"
#include "../Context/Context.h"
#include "../Scene/Scene.h"
#include "Game.h"

#include "../Graphics/RenderState.h"


EXPORT_CLASS EngineInstance
{
public:
	EngineInstance();
	~EngineInstance();

	DRESULT Dettach();
	DRESULT Start();
	DRESULT AttachGame(Game* _game);
	DRESULT AttachContext(Context* _context);

	Context* _currentContext;

	Game* _game;

	bool IsAttached;



};

