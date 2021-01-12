#pragma once


#include "Core/Common/CoreCommon.h"
#include "Core/Framework/Common/FDelegate.h"


#include "KeyInputBinding.h"


EXPORT(class, ControlComponent)
{
public:

	template<class TUserClass, typename FKeyEventBindingDelegate::FMemFuncSignature<TUserClass> TMethod>
	void BindKeyEvent(TUserClass*  obj)
	{
		KeyEventBindings.Add(FKeyEventBindingDelegate::Make<TUserClass, TMethod>(obj));
	}



	FArray<FKeyEventBindingDelegate> KeyEventBindings;
};

EXPORT(class, ControllableObject)
{
public:
	ControlComponent ControlComponent;



	ControllableObject()
	{

	}

};

