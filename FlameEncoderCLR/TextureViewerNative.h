#pragma once


namespace FlameEncoderCLR
{
	struct NativeTextureDesc
	{
		int width;
		int height;
		unsigned char* data;
		size_t dataSize;
	};

	struct FRIData
	{
		void* data;
		size_t ByteSize;
	};

	class TextureViewerNative
	{
	public:

		TextureViewerNative(void* hwnd, int width, int height);

		void Frame();
		bool IsContextActive();
		void PollClose();

		void SetMaterial(NativeTextureDesc* textureDesc, int textures);
		void RenderIrradiance(FRIData* data);

		class TextureViewerEngine* gameApp;
	};

}