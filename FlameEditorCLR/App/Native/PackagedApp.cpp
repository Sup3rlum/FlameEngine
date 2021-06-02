#include "PackagedApp.h"

#include "NativeEditorApp.h"
#include "Platform/Windows/Win32Context.h"

namespace FlameEditorCLR
{
	PackagedApp::PackagedApp(void* hwnd, int x, int y, int width, int height)
	{
		appInstance = new EditorApplication(new Win32Context((HWND)hwnd, x, y, width, height));
	}

	void PackagedApp::Frame()
	{
		appInstance->Frame();
	}
	void PackagedApp::Load()
	{
		appInstance->Load();
	}
	bool PackagedApp::IsContextActive()
	{
		return appInstance->IsContextActive();
	}
}