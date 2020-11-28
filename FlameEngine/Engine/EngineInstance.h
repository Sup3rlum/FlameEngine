#pragma once

#include "../dll/nchfx.h"
#include "../dll/Version.h"

#include "../Context/ContextDescription.h"
#include "../Context/GLFWContext.h"
#include "../Context/Context.h"
#include "../Scene/Scene.h"
#include "Game.h"

#include "../Graphics/RenderState.h"
#include "../Graphics/Renderer/DeferredRenderer.h"



EXPORT_CLASS EngineInstance
{
public:
	EngineInstance();
	~EngineInstance();

	FLRESULT Dettach();
	FLRESULT Start();
	FLRESULT AttachGame(Game* _game);
	FLRESULT AttachContext(Context* _context);
	static void key_callback_dispatch(GLFWwindow* win, int key, int scancode, int action, int mods);

	Context* _currentContext;
	Game* _game;

	DeferredRenderer* mRenderer;


	bool IsAttached;


	static EngineInstance* _handlingInstance;

};

