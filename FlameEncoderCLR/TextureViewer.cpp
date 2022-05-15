
#include "TextureViewerNative.h"
#include "TextureViewer.h"

using namespace FlameEncoderCLR;

using namespace System;
using namespace System::Runtime::InteropServices;

TextureViewer::TextureViewer(IntPtr hwnd, int width, int height) : 
	ManagedObject<TextureViewerNative>(new TextureViewerNative(hwnd.ToPointer(), width, height))
{

}

bool TextureViewer::IsContextActive()
{
	return this->m_Instance->IsContextActive();
}

void TextureViewer::Frame()
{
	this->m_Instance->Frame();
}

void TextureViewer::PollClose()
{
	this->m_Instance->PollClose();
}

void TextureViewer::SetMaterial(array<TextureDesc^>^ maps)
{
	NativeTextureDesc desc[4];

	for (int i = 0; i < maps->Length; i++)
	{
		desc[i].width = maps[i]->width;
		desc[i].height = maps[i]->height;
		
		auto dataSize = maps[i]->Data->Length;

		desc[i].data = (unsigned char*)Marshal::AllocHGlobal(dataSize).ToPointer();
		desc[i].dataSize = dataSize;

		Marshal::Copy(maps[i]->Data, 0, IntPtr(desc[i].data), dataSize);
	}

	this->m_Instance->SetMaterial(desc, maps->Length);


	for (int i = 0; i < maps->Length; i++)
	{
		Marshal::FreeHGlobal(IntPtr(desc[i].data));
	}
}

void TextureViewer::RenderIrradiance(array<TextureDesc^>^ maps)
{
	FRIData desc[6];

	for (int i = 0; i < maps->Length; i++)
	{
		auto dataSize = maps[i]->Data->Length;

		desc[i].data = (unsigned char*)Marshal::AllocHGlobal(dataSize).ToPointer();
		desc[i].ByteSize = dataSize;

		Marshal::Copy(maps[i]->Data, 0, IntPtr(desc[i].data), dataSize);
	}

	this->m_Instance->RenderIrradiance(desc);


	for (int i = 0; i < maps->Length; i++)
	{
		Marshal::FreeHGlobal(IntPtr(desc[i].data));
	}
}