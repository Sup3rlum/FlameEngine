#pragma once

#include <GLFW/glfw3.h>

#include "Core/Common/CoreCommon.h"
#include "../Input/KeyEventArgs.h"
#include "ContextDescription.h"
#include "Context.h"

EXPORT(class,  GLFWContext) : public Context
{
public:

	void Initialize(ContextDescription* _contextDescription) override;
	void SwapChain() override;
	void Destroy() override;
	void PollClose() override;

	void GetCursorPosition(double* x, double* y) override;
	void SetCursorPosition(double x, double y) override;


	void SetCursorVisible(bool v) override;

	KeyState GetKeyState(Keys key) override;
	MouseButtonState GetMouseButtonState(MouseButton mbutton) override;

	GLFWwindow* _windowHandle;


	static GLFWContext* handlingInstance;

private:
	static void key_callback_dispatch(GLFWwindow* win, int key, int scancode, int action, int mods);
};

