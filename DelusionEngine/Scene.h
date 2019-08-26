#pragma once


#include "Camera.h"
#include "Shader.h"
#include "RenderBatch.h"
#include "FrameBuffer.h"
#include "DebugView.h"

EXPORT_CLASS Scene
{
public:
	Scene(BaseContext* _context);
	~Scene();

	void Update(FrameTime* _frTime);
	void Render(FrameTime* _frTime);

	Camera* _camera;
	

};

