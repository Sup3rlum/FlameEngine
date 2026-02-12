#pragma once

#include "ManagedObject.h"
#include "EditorAppNative.h"

using namespace System;

namespace FlameEditorCLR
{

	public ref class EditorApp : public ManagedObject<EditorAppNative>
	{
	public:

		EditorApp(IntPtr hwnd, int width, int height);

		void Frame();
		bool IsContextActive();

		void PollClose();
	};
}
