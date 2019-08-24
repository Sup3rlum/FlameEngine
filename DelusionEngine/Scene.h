#pragma once


#include "Camera.h"
#include "Shader.h"
#include "RenderBatch.h"
#include "FrameBuffer.h"
#include "DebugView.h"

class DELUSION_DLL Scene
{
public:
	Scene(ContextDescription* params);
	~Scene();

	void Update(FrameTime* _frTime);
	void Render(FrameTime* _frTime);

	Camera* _camera;
	
	ContextDescription* _contentDescr;

};

