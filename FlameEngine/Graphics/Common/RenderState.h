#pragma once

#include "../../dll/nchfx.h"

using namespace std;

EXPORT_ENUM BlendFunc
{
	Zero						= 0,
	One							= 1,
	SourceColor					= 0x0300,
	OneMinusSourceColor			= 0x0301,
	DestinationColor			= 0x0306,
	OneMinusDestinationColor	= 0x0307,

	SourceAlpha					= 0x0302,
	OneMinusSourceAlpha			= 0x0303,
	DestinationAlpha			= 0x0304,
	OneMinusDestinationAlpha	= 0x0305,

	ConstantColor				= 0x8001,
	OneMinusConstantColor		= 0x8002,

	ConstantAlpha				= 0x8003,
	OneMinusConstantAlpha		= 0x8004

};
EXPORT_ENUM DepthFunc
{
	Always			= 0x0207,
	Never			= 0x0200,
	Less			= 0x0201,
	Equal			= 0x0202,
	LessOrEqual		= 0x0203,
	Greater			= 0x0204,
	GeaterOrEqal	= 0x0206,
	NotEqual		= 0x0205
};
EXPORT_ENUM CullState
{
	FrontAndBack	= 0x0408,
	Front			= 0x0404,
	Back			= 0x0405
};


EXPORT(class,  RenderState)
{
public:
	BlendFunc SourceBlend;
	BlendFunc DestinationBlend;

	DepthFunc DepthFunction;

	CullState CullState;


	static void Push(RenderState* _state);
	static void Pop();
	static RenderState Top();

	static void InitilizeSurface();

private:
	static stack<RenderState> States;
	static void ApplyState();
};

