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

	template<class TUserClass, typename FMouseKeyEventBindingDelegate::FMemFuncSignature<TUserClass>::FuncPtr TMethod>
	void BindMouseEvent(TUserClass* obj)
	{
		MouseEventBindings.Add(FMouseKeyEventBindingDelegate::Make<TUserClass, TMethod>(obj));
	}


	bool IsKeyDown(FKeyboardKeys key);
	bool IsMouseButtonDown(FMouseButton button);

	FArray<FKeyEventBindingDelegate> KeyEventBindings;
	FArray<FMouseKeyEventBindingDelegate> MouseEventBindings;
};




//RegisterEntityComponent(ControlComponent, "ControlComponent");