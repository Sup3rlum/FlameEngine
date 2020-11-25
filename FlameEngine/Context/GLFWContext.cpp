#include "GLFWContext.h"


void GLFWContext::Initialize(ContextDescription* _contextDescription)
{

	// GLFW Context

	if (!glfwInit())
	{
		FLAME_MSGBOX_ERROR(L"Failed to initialize GLFW\n");
	}

	glfwWindowHint(GLFW_SAMPLES,				_contextDescription->contextSampleCount);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,	_contextDescription->contextMajorVersion);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,	_contextDescription->contextMinorVersion);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT,	_contextDescription->contextCompatibility);
	glfwWindowHint(GLFW_OPENGL_PROFILE,			(int)_contextDescription->contextProfile);

	_windowHandle = glfwCreateWindow(_contextDescription->width, _contextDescription->height, "Flame", _contextDescription->fullscreen ? glfwGetPrimaryMonitor() : NULL, NULL);

	if (_windowHandle == NULL)
	{
		fprintf(stderr, "OpenGL 4.5 is not supported by this PC");
		this->Destroy();
	}

	glfwMakeContextCurrent(_windowHandle);

	glfwSwapInterval(0);

	glfwSetInputMode(_windowHandle, GLFW_STICKY_KEYS, GL_TRUE);
	

	SetCursorVisible(false);




	glewExperimental = true;

	if (glewInit() != GLEW_OK)
	{
		FLAME_MSGBOX_ERROR(L"Failed to initialize GLEW");
	}


	__super::Initialize(_contextDescription);
}
void GLFWContext::SwapChain()
{
	__super::SwapChain();

	glfwSwapBuffers(_windowHandle);
	glfwPollEvents();

	shouldTerminate = glfwGetKey(_windowHandle, GLFW_KEY_ESCAPE) == GLFW_PRESS ||
		glfwWindowShouldClose(_windowHandle) != 0;
}
void GLFWContext::GetCursorPosition(double* x, double* y)
{
	glfwGetCursorPos(_windowHandle, x, y);
}
void GLFWContext::SetCursorPosition(double x, double y)
{
	glfwSetCursorPos(_windowHandle, x, y);
}

KeyState GLFWContext::GetKeyState(Keys key)
{
	return (KeyState)glfwGetKey(_windowHandle, (int)key);
}
void GLFWContext::Destroy()
{
	glfwTerminate();
}
void GLFWContext::PollClose()
{
	__super::PollClose();
}
void GLFWContext::SetCursorVisible(bool visible)
{
	if (visible)
	{
		glfwSetInputMode(_windowHandle, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	}
	else
	{
		glfwSetInputMode(_windowHandle, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
	}
}

MouseButtonState GLFWContext::GetMouseButtonState(MouseButton mbutton)
{
	return (MouseButtonState)glfwGetMouseButton(_windowHandle, (int)mbutton);
}