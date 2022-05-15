#pragma once

#include "ManagedObject.h"
#include "TextureViewerNative.h"

using namespace System;

namespace FlameEncoderCLR 
{
	public ref struct TextureDesc
	{
		int width;
		int height;
		array<Byte>^ Data;
	};

	public ref class TextureViewer : public ManagedObject<TextureViewerNative>
	{
	public:

		TextureViewer(IntPtr hwnd, int width, int height);

		void Frame();
		bool IsContextActive();

		void PollClose();
		void SetMaterial(array<TextureDesc^>^ maps);
		void RenderIrradiance(array<TextureDesc^>^ maps);
	};
}
