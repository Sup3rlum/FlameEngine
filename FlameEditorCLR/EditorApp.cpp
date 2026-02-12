
#include "EditorAppNative.h"
#include "EditorApp.h"

using namespace FlameEditorCLR;

using namespace System;
using namespace System::Runtime::InteropServices;

EditorApp::EditorApp(IntPtr hwnd, int width, int height) :
	ManagedObject<EditorAppNative>(new EditorAppNative(hwnd.ToPointer(), width, height))
{

}

bool EditorApp::IsContextActive()
{
	return this->m_Instance->IsContextActive();
}

void EditorApp::Frame()
{
	this->m_Instance->Frame();
}

void EditorApp::PollClose()
{
	this->m_Instance->PollClose();
}