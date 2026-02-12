#pragma once

namespace FlameEditorCLR
{
	struct FRIData
	{
		void* data;
		size_t ByteSize;
	};

	class EditorAppNative
	{
	public:

		EditorAppNative(void* hwnd, int width, int height);

		void Frame();
		bool IsContextActive();
		void PollClose();

		class EditorEngineApp* gameApp;
	};

}