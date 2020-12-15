#pragma once

#include "Core/Common/CoreCommon.h"

#include "Context/ContextDescription.h"
#include "Context/GLFWContext.h"
#include "Context/Context.h"
#include "Core/Engine/Scene/Common/Scene.h"
#include "Core/Engine/GameSystem/Game.h"

#include "../Graphics/Common/RenderState.h"
#include "../Graphics/Renderer/DeferredRenderer.h"



EXPORT(class,  EngineInstance)
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

