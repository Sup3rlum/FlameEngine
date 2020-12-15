#pragma once

#include "../dll/nchfx.h"
#include "Keyboard.h"

EXPORT(struct,  KeyEventArgs)
{
public:
	Keys Key;
	KeyState keyState;

	KeyEventArgs(Keys k, KeyState ks) : Key(k), keyState(ks) {}

};

