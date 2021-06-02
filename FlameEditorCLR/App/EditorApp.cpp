#include "EditorApp.h"


namespace FlameEditorCLR
{
	EditorApp::EditorApp(IntPtr hwnd, int x, int y, int width, int height) : 
		ManagedObject(new PackagedApp(hwnd.ToPointer(), x, y, width, height))
	{

	}

	void EditorApp::Frame()
	{
		m_Instance->Frame();
	}
	void EditorApp::Load()
	{
		m_Instance->Load();
	}
	bool EditorApp::IsContextActive()
	{
		return m_Instance->IsContextActive();
	}
}