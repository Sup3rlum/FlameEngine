#pragma once

#pragma once

#include "Core/Common/CoreCommon.h"
#include "Core/Math/Module.h"

struct Color;

EXPORT(struct, Color32)
{
	union
	{
		struct { float r, g, b, a; };
		struct { float r;  FVector2 gb; float a; };
		struct { FVector2 rg; FVector2 ba; };
		struct { FVector3 rgb; float a; };
		struct { float r; FVector3 gba; };
	};

public:
	Color32(float r, float g, float b);
	Color32(float r, float g, float b, float a);
	Color32();

	Color32(const Color& color);
	Color32(const Color32& other);

	Color32& operator=(const Color32& other);

	static Color32 Invert(const Color32& color)
	{
		return Color32(1.0f - color.r, 1.0f - color.g, 1.0f - color.b, color.a);
	}

	static Color32 Alicelue;
	static Color32 AntiqueWhite;
	static Color32 Aqua;
	static Color32 Aquamarine;
	static Color32 Azure;
	static Color32 Beige;
	static Color32 Bisque;
	static Color32 Black;
	static Color32 BlanchedAlmond;
	static Color32 Blue;
	static Color32 Blueviolet;
	static Color32 Brown;
	static Color32 Burlywood;
	static Color32 CadetBlue;
	static Color32 Chartreuse;
	static Color32 Chocolate;
	static Color32 Coral;
	static Color32 CornflowerBlue;
	static Color32 Cornsilk;
	static Color32 CosmicLatte;
	static Color32 Crimson;
	static Color32 Cyan;
	static Color32 DarkBlue;
	static Color32 DarkCyan;
	static Color32 DarkGoldenrod;
	static Color32 DarkGray;
	static Color32 DarkGreen;
	static Color32 DarkKhaki;
	static Color32 DarkMagenta;
	static Color32 DarkOliveGreen;
	static Color32 DarkOrange;
	static Color32 DarkOrchid;
	static Color32 DarkRed;
	static Color32 DarkSalmon;
	static Color32 DarkSeaGreen;
	static Color32 DarkSlateBlue;
	static Color32 DarkSlateGray;
	static Color32 DarkTurquoise;
	static Color32 DarkViolet;
	static Color32 DeepPink;
	static Color32 DeepSkyBlue;
	static Color32 DimGray;
	static Color32 DodgerBlue;
	static Color32 FireBrick;
	static Color32 FloralWhite;
	static Color32 ForestGreen;
	static Color32 Fuchsia;
	static Color32 Gainsboro;
	static Color32 GhostWhite;
	static Color32 Gold;
	static Color32 Goldenrod;
	static Color32 Gray;
	static Color32 Green;
	static Color32 GreenYellow;
	static Color32 Honeydew;
	static Color32 HotPink;
	static Color32 IndianRed;
	static Color32 Indigo;
	static Color32 Ivory;
	static Color32 Khaki;
	static Color32 Lavender;
	static Color32 LavenderBlush;
	static Color32 LawnGreen;
	static Color32 LemonChiffon;
	static Color32 LightBlue;
	static Color32 LightCoral;
	static Color32 LightCyan;
	static Color32 LightGoldenrodYellow;
	static Color32 LightGray;
	static Color32 LightGreen;
	static Color32 LightPink;
	static Color32 LightSalmon;
	static Color32 LightSeaGreen;
	static Color32 LightSkyBlue;
	static Color32 LightSlateGray;
	static Color32 LightSteelBlue;
	static Color32 LightYellow;
	static Color32 Lime;
	static Color32 LimeGreen;
	static Color32 Linen;
	static Color32 Magenta;
	static Color32 Maroon;
	static Color32 MediumAquaMarine;
	static Color32 MediumBlue;
	static Color32 MediumOrchid;
	static Color32 MediumPurple;
	static Color32 MediumSeaGreen;
	static Color32 MediumSlateBlue;
	static Color32 MediumSpringGreen;
	static Color32 MediumTurquoise;
	static Color32 MediumVioletRed;
	static Color32 MidnightBlue;
	static Color32 MintCream;
	static Color32 MistyRose;
	static Color32 Moccasin;
	static Color32 NavajoWhite;
	static Color32 Navy;
	static Color32 Oldlace;
	static Color32 Olive;
	static Color32 OliveDrab;
	static Color32 Orange;
	static Color32 OrangeRed;
	static Color32 Orchid;
	static Color32 PaleAzure;
	static Color32 PaleGoldenrod;
	static Color32 PaleGreen;
	static Color32 PaleTurquoise;
	static Color32 PaleVioletRed;
	static Color32 PapayaWhip;
	static Color32 PeachPuff;
	static Color32 Peru;
	static Color32 Pink;
	static Color32 Plum;
	static Color32 PowderBlue;
	static Color32 Purple;
	static Color32 Red;
	static Color32 RosyBrown;
	static Color32 RoyalBlue;
	static Color32 SaddleBrown;
	static Color32 Salmon;
	static Color32 SandyBrown;
	static Color32 SeaGreen;
	static Color32 SeaShell;
	static Color32 Sienna;
	static Color32 Silver;
	static Color32 SkyBlue;
	static Color32 SlateBlue;
	static Color32 SlateGray;
	static Color32 Snow;
	static Color32 SpringGreen;
	static Color32 SteelBlue;
	static Color32 Tan;
	static Color32 Teal;
	static Color32 Thistle;
	static Color32 Tomato;
	static Color32 Turquoise;
	static Color32 Violet;
	static Color32 Wheat;
	static Color32 White;
	static Color32 WhiteSmoke;
	static Color32 Yellow;
	static Color32 YellowGreen;

	static Color32 Transparent;
};
