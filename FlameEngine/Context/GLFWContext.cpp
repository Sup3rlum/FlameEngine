#include "GLFWContext.h"
#include <intrin.h>

void GLFWContext::Initialize(ContextDescription* _contextDescription)
{
	int CPUInfo[4] = { -1 };
	unsigned   nExIds, i = 0;
	char CPUBrandString[0x40];
	// Get the information associated with each extended ID.
	__cpuid(CPUInfo, 0x80000000);
	nExIds = CPUInfo[0];
	for (i = 0x80000000; i <= nExIds; ++i)
	{
		__cpuid(CPUInfo, i);
		// Interpret CPU brand string
		if (i == 0x80000002)
			memcpy(CPUBrandString, CPUInfo, sizeof(CPUInfo));
		else if (i == 0x80000003)
			memcpy(CPUBrandString + 16, CPUInfo, sizeof(CPUInfo));
		else if (i == 0x80000004)
			memcpy(CPUBrandString + 32, CPUInfo, sizeof(CPUInfo));
	}

	MEMORYSTATUSEX statex;
	statex.dwLength = sizeof(statex);
	GlobalMemoryStatusEx(&statex);


	SYSTEM_INFO sysinfo;
	GetSystemInfo(&sysinfo);




	Runtime::handlingInstance = new Runtime();
	Runtime::handlingInstance->sysInfo.cpuInfo.oemString = STRING(CPUBrandString);
	Runtime::handlingInstance->sysInfo.cpuInfo.numCores = sysinfo.dwNumberOfProcessors;

	Runtime::handlingInstance->sysInfo.memoryInfo.size = statex.ullTotalPhys;


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

	GLint vMemoryKb = 0;
	glGetIntegerv(GL_GPU_MEMORY_INFO_TOTAL_AVAILABLE_MEMORY_NVX, &vMemoryKb);

	
	Runtime::handlingInstance->sysInfo.gpuInfo.oemVendor = STRING((char*)glGetString(GL_VENDOR));
	Runtime::handlingInstance->sysInfo.gpuInfo.oemString = STRING((char*)glGetString(GL_RENDERER));
	Runtime::handlingInstance->sysInfo.gpuInfo.driverString = STRING((char*)glGetString(GL_VERSION));
	Runtime::handlingInstance->sysInfo.gpuInfo.vMemory = vMemoryKb * 1024;


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