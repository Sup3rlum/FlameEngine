#pragma once

#include "CLR/ManagedObject.h"
#include "Native/PackagedApp.h"


#pragma managed

using namespace System;


namespace FlameEditorCLR
{

	public ref class EditorApp : public ManagedObject<PackagedApp>
	{
	public:
		EditorApp(IntPtr hwnd, int x, int y, int width, int height);
		void Load();
		void Frame();
		bool IsContextActive();

	};

}