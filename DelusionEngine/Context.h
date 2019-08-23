#pragma once



#include "nchfx.h"

#include "FrameTime.h"
#include "Scene.h"

using namespace glm;



class DELUSION_DLL Context
{
public:
	static GLFWwindow* _window;
	static ContextParameters* _contextParameters;
	static FrameTime* _frameTime;
	static Scene* _scene;

	static int Initialize(ContextParameters* _params);
	static void Frame(FrameTime* _frTime);
	static void Update(FrameTime* _frTime);

	static int InitGLEW();
	static int InitGLFW();

};

