#include "Context.h"



ContextDescription*	Context::_contextDescr		= NULL;
FrameTime*			Context::_frameTime			= NULL;
Scene*				Context::_scene				= NULL;


int Context::Initialize(ContextDescription* _params)
{
	_contextDescr = _params;

	InitGLFW();
	InitGLEW();

	_frameTime = new FrameTime();
	_scene = new Scene(_params);

	// Main Loop
	do
	{
		_frameTime->DeltaTime = glfwGetTime() - _frameTime->TotalTime;
		_frameTime->TotalTime = glfwGetTime();
		_frameTime->EllapsedFrames++;

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);
		glCullFace(GL_FRONT);




		Update(_frameTime);
		Frame(_frameTime);

		glfwSwapBuffers(_contextDescr->window);
		glfwPollEvents();

	} 
	while (glfwGetKey(_contextDescr->window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
		glfwWindowShouldClose(_contextDescr->window) == 0);

	return 0;
}
void Context::Frame(FrameTime* _frTime)
{
	_scene->Render(_frameTime);
}
void Context::Update(FrameTime* _frTime)
{
	_scene->Update(_frameTime);
}

int Context::InitGLEW()
{
	glewExperimental = true;

	GLuint _initResult = glewInit();

	if (_initResult != GLEW_OK)
	{
		fprintf(stderr, "Failed to initialize GLEW\n");
		return -1;
	}
	else
	{
		return 0;
	}
}
int Context::InitGLFW()
{

	if (!glfwInit())
	{
		fprintf(stderr, "Failed to initialize GLFW\n");
		return -1;
	}

	// GLFW Context

	glfwWindowHint(GLFW_SAMPLES,					_contextDescr->contextSampleCount);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,		_contextDescr->contextMajorVersion);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,		_contextDescr->contextMinorVersion);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT,		_contextDescr->contextCompatibility);
	glfwWindowHint(GLFW_OPENGL_PROFILE,				(int)_contextDescr->contextProfile);

	_contextDescr->window = glfwCreateWindow(_contextDescr->width, _contextDescr->height, "NativeCraft", _contextDescr->fullscreen ? glfwGetPrimaryMonitor() : NULL, NULL);

	if (_contextDescr->window == NULL)
	{
		fprintf(stderr, "OpenGL 4.5 is not supported by this PC");
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(_contextDescr->window);

	glfwSwapInterval(0);

	glfwSetInputMode(_contextDescr->window, GLFW_STICKY_KEYS, GL_TRUE);
	glfwSetInputMode(_contextDescr->window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
}