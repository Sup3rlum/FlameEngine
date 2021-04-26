#pragma once

enum class EMaterialWrap
{
	Repeat,
	ClampEdge,
	ClampBorder,
	MirroredRepeat,
	MirroredClampEdge
};


enum class EMaterialFilter
{
	Nearest,
	Bilinear,
	Trilinear,
	Anisotropic4,
	Anisotropic8,
};