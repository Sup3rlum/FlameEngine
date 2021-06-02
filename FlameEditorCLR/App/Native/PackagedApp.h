#pragma once


#pragma unmanaged

namespace FlameEditorCLR
{
	class EditorApplication;

	class PackagedApp
	{
	public:
		PackagedApp(void* hwnd, int x, int y, int width, int height);

		bool IsContextActive();
		void Frame();
		void Load();

		EditorApplication* appInstance;
	};

}