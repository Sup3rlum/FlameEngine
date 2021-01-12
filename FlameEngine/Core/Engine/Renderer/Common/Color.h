#pragma once

#include "Core/Common/CoreCommon.h"
#include "Core/Math/Module.h"



EXPORT(struct, Color) : FVector4
{
public:
	Color(byte r, byte g, byte b);
	Color(byte r, byte g, byte b, byte a);

	Color(float r, float g, float b) : FVector4(r, g, b, 1) {}
	Color(float r, float g, float b, float a) : FVector4(r, g, b, a) {}


	Color(uint32 hex);
	Color();


	static Color Invert(const Color& color)
	{
		return Color(1 - color.r, 1 - color.g, 1 - color.b, color.a);
	}




	static Color Alicelue;
	static Color AntiqueWhite;
	static Color Aqua;
	static Color Aquamarine;
	static Color Azure;
	static Color Beige;
	static Color Bisque;
	static Color Black;
	static Color BlanchedAlmond;
	static Color Blue;
	static Color Blueviolet;
	static Color Brown;
	static Color Burlywood;
	static Color CadetBlue;
	static Color Chartreuse;
	static Color Chocolate;
	static Color Coral;
	static Color CornflowerBlue;
	static Color Cornsilk;
	static Color CosmicLatte;
	static Color Crimson;
	static Color Cyan;
	static Color DarkBlue;
	static Color DarkCyan;
	static Color DarkGoldenrod;
	static Color DarkGray;
	static Color DarkGreen;
	static Color DarkKhaki;
	static Color DarkMagenta;
	static Color DarkOliveGreen;
	static Color DarkOrange;
	static Color DarkOrchid;
	static Color DarkRed;
	static Color DarkSalmon;
	static Color DarkSeaGreen;
	static Color DarkSlateBlue;
	static Color DarkSlateGray;
	static Color DarkTurquoise;
	static Color DarkViolet;
	static Color DeepPink;
	static Color DeepSkyBlue;
	static Color DimGray;
	static Color DodgerBlue;
	static Color FireBrick;
	static Color FloralWhite;
	static Color ForestGreen;
	static Color Fuchsia;
	static Color Gainsboro;
	static Color GhostWhite;
	static Color Gold;
	static Color Goldenrod;
	static Color Gray;
	static Color Green;
	static Color GreenYellow;
	static Color Honeydew;
	static Color HotPink;
	static Color IndianRed;
	static Color Indigo;
	static Color Ivory;
	static Color Khaki;
	static Color Lavender;
	static Color LavenderBlush;
	static Color LawnGreen;
	static Color LemonChiffon;
	static Color LightBlue;
	static Color LightCoral;
	static Color LightCyan;
	static Color LightGoldenrodYellow;
	static Color LightGray;
	static Color LightGreen;
	static Color LightPink;
	static Color LightSalmon;
	static Color LightSeaGreen;
	static Color LightSkyBlue;
	static Color LightSlateGray;
	static Color LightSteelBlue;
	static Color LightYellow;
	static Color Lime;
	static Color LimeGreen;
	static Color Linen;
	static Color Magenta;
	static Color Maroon;
	static Color MediumAquaMarine;
	static Color MediumBlue;
	static Color MediumOrchid;
	static Color MediumPurple;
	static Color MediumSeaGreen;
	static Color MediumSlateBlue;
	static Color MediumSpringGreen;
	static Color MediumTurquoise;
	static Color MediumVioletRed;
	static Color MidnightBlue;
	static Color MintCream;
	static Color MistyRose;
	static Color Moccasin;
	static Color NavajoWhite;
	static Color Navy;
	static Color Oldlace;
	static Color Olive;
	static Color OliveDrab;
	static Color Orange;
	static Color OrangeRed;
	static Color Orchid;
	static Color PaleGoldenrod;
	static Color PaleGreen;
	static Color PaleTurquoise;
	static Color PaleVioletRed;
	static Color PapayaWhip;
	static Color PeachPuff;
	static Color Peru;
	static Color Pink;
	static Color Plum;
	static Color PowderBlue;
	static Color Purple;
	static Color Red;
	static Color RosyBrown;
	static Color RoyalBlue;
	static Color SaddleBrown;
	static Color Salmon;
	static Color SandyBrown;
	static Color SeaGreen;
	static Color SeaShell;
	static Color Sienna;
	static Color Silver;
	static Color SkyBlue;
	static Color SlateBlue;
	static Color SlateGray;
	static Color Snow;
	static Color SpringGreen;
	static Color SteelBlue;
	static Color Tan;
	static Color Teal;
	static Color Thistle;
	static Color Tomato;
	static Color Turquoise;
	static Color Violet;
	static Color Wheat;
	static Color White;
	static Color WhiteSmoke;
	static Color Yellow;
	static Color YellowGreen;

	static Color Transparent;
};
