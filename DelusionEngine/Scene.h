#pragma once


#include "Camera.h"
#include "Shader.h"
#include "RenderBatch.h"
#include "FrameBuffer.h"
#include "DebugView.h"
#include "Context.h"

EXPORT_CLASS Scene
{
public:
	Scene(Context* _context);
	~Scene();

	void Update();
	void Render();

	Camera* _camera;
	

};

