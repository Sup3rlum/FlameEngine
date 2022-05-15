
#include "InputComponent.h"

bool Input::IsKeyDown(FKeyboardKeys key)
{
	return (GetAsyncKeyState((int32)key) & 0x8000) != 0;
}

bool Input::IsMouseButtonDown(FMouseButton button)
{
	return (GetAsyncKeyState((int32)button) & 0x8000) != 0;
}