#pragma once

#include "Platform/OpenGL/gl.h"
#include "Core/Common/CoreCommon.h"


enum class OpenGLBlendFunc
{
	Zero = 0,
	One = 1,
	SourceColor = 0x0300,
	OneMinusSourceColor = 0x0301,
	DestinationColor = 0x0306,
	OneMinusDestinationColor = 0x0307,

	SourceAlpha = 0x0302,
	OneMinusSourceAlpha = 0x0303,
	DestinationAlpha = 0x0304,
	OneMinusDestinationAlpha = 0x0305,

	ConstantColor = 0x8001,
	OneMinusConstantColor = 0x8002,

	ConstantAlpha = 0x8003,
	OneMinusConstantAlpha = 0x8004

};
enum class OpenGLDepthFunc
{
	Always = 0x0207,
	Never = 0x0200,
	Less = 0x0201,
	Equal = 0x0202,
	LessOrEqual = 0x0203,
	Greater = 0x0204,
	GeaterOrEqal = 0x0206,
	NotEqual = 0x0205
};
enum class OpenGLCullState
{
	FrontAndBack = 0x0408,
	Front = 0x0404,
	Back = 0x0405
};


class OpenGLRenderState
{
public:
	OpenGLBlendFunc SourceBlend;
	OpenGLBlendFunc DestinationBlend;
	OpenGLDepthFunc DepthFunction;
	OpenGLCullState CullState;
};


struct OpenGL
{
	static FStack<OpenGLRenderState> RenderStateStack;


public:
	FORCEINLINE static void ClearColor(Color color)
	{
		glClearColor(color.r, color.g, color.b, color.a);
	}
	FORCEINLINE static void GenBuffers(uint32 bufferNum, GLuint* buffers)
	{
		glGenBuffers(bufferNum, buffers);
	}
	FORCEINLINE static void BufferData(uint32 buffer, uint32 type, void* data, size_t byteSize, uint32 drawAccess)
	{
		glBindBuffer(type, buffer);
		glBufferData(type, byteSize, data, drawAccess);
	}
	FORCEINLINE static void GenTextures(uint32 textureCount, GLuint* textures)
	{
		glGenTextures(textureCount, textures);
	}
	FORCEINLINE static void TexData2D(uint32 texture)
	{

	}


	static bool DrvSupportsBinaryShaderFormat()
	{
		GLint formats = 0;
		glGetIntegerv(GL_NUM_PROGRAM_BINARY_FORMATS, &formats);

		return formats != 0;
	}
};