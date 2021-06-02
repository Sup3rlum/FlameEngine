#pragma once

#include "EntityComponent/EntityComponent.h"
#include "KeyInputBinding.h"

EXPORT(struct, ControlComponent)
{
public:

	template<class TUserClass, typename FKeyEventBindingDelegate::FMemFuncSignature<TUserClass>::FuncPtr TMethod>
	void BindKeyEvent(TUserClass * obj)
	{
		KeyEventBindings.Add(FKeyEventBindingDelegate::Make<TUserClass, TMethod>(obj));
	}


	bool IsKeyDown(FKeyboardKeys key)
	{
		return (GetAsyncKeyState((int32)key) & 0x8000) != 0;
	}

	bool IsMouseButtonDown(FMouseButton button)
	{
		return (GetAsyncKeyState((int32)button) & 0x8000) != 0;
	}

	FArray<FKeyEventBindingDelegate> KeyEventBindings;
};




//RegisterEntityComponent(ControlComponent, "ControlComponent");