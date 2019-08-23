#pragma once


#include "Camera.h"
#include "Shader.h"
#include "RenderBatch.h"
#include "FrameBuffer.h"
#include "DebugView.h"

class DELUSION_DLL Scene
{
public:
	Scene(ContextParameters* params);
	~Scene();

	void Update(GLFWwindow* _win, FrameTime* _frTime);
	void Render(FrameTime* _frTime);

	Camera* _camera;
	RenderBatch* _rb;

	ContextParameters* _params;

};

