#include "Color32.h"
#include "Color8.h"

Color32::Color32(float r, float g, float b) : Color32(r, g, b, 1.0f)
{
}

Color32::Color32(float r, float g, float b, float a) :
	r(r),
	g(g),
	b(b),
	a(a)
{
}

Color32::Color32() : Color32(0, 0, 0, 0)
{
}

Color32::Color32(const Color32& color) :
	r(color.r),
	g(color.g),
	b(color.b),
	a(color.a)
{
}

Color32::Color32(const Color& color) :
	Color32(
		color.r / 255.0f,
		color.g / 255.0f,
		color.b / 255.0f,
		color.a / 255.0f
	)
{
}

Color32& Color32::operator=(const Color32& other)
{
	r = other.r;
	g = other.g;
	b = other.b;
	a = other.a;

	return *this;
}



Color32 Color32::Alicelue = Color(240, 248, 255);
Color32 Color32::AntiqueWhite = Color(250, 235, 215);
Color32 Color32::Aqua = Color(0, 255, 255);
Color32 Color32::Aquamarine = Color(127, 255, 212);
Color32 Color32::Azure = Color(0, 127, 255);
Color32 Color32::Beige = Color(245, 245, 220);
Color32 Color32::Bisque = Color(255, 228, 196);
Color32 Color32::Black = Color(0, 0, 0);
Color32 Color32::BlanchedAlmond = Color(255, 235, 205);
Color32 Color32::Blue = Color(0, 0, 255);
Color32 Color32::Blueviolet = Color(138, 43, 226);
Color32 Color32::Brown = Color(165, 42, 42);
Color32 Color32::Burlywood = Color(222, 184, 135);
Color32 Color32::CadetBlue = Color(95, 158, 160);
Color32 Color32::Chartreuse = Color(127, 255, 0);
Color32 Color32::Chocolate = Color(210, 105, 30);
Color32 Color32::Coral = Color(255, 127, 80);
Color32 Color32::CornflowerBlue = Color(100, 149, 237);
Color32 Color32::Cornsilk = Color(255, 248, 220);
Color32 Color32::CosmicLatte = Color(255, 248, 231);
Color32 Color32::Crimson = Color(220, 20, 60);
Color32 Color32::Cyan = Color(0, 255, 255);
Color32 Color32::DarkBlue = Color(0, 0, 139);
Color32 Color32::DarkCyan = Color(0, 139, 139);
Color32 Color32::DarkGoldenrod = Color(184, 134, 11);
Color32 Color32::DarkGray = Color(169, 169, 169);
Color32 Color32::DarkGreen = Color(0, 100, 0);
Color32 Color32::DarkKhaki = Color(189, 183, 107);
Color32 Color32::DarkMagenta = Color(139, 0, 139);
Color32 Color32::DarkOliveGreen = Color(85, 107, 47);
Color32 Color32::DarkOrange = Color(255, 140, 0);
Color32 Color32::DarkOrchid = Color(153, 50, 204);
Color32 Color32::DarkRed = Color(139, 0, 0);
Color32 Color32::DarkSalmon = Color(233, 150, 122);
Color32 Color32::DarkSeaGreen = Color(143, 188, 139);
Color32 Color32::DarkSlateBlue = Color(72, 61, 139);
Color32 Color32::DarkSlateGray = Color(47, 79, 79);
Color32 Color32::DarkTurquoise = Color(0, 206, 209);
Color32 Color32::DarkViolet = Color(148, 0, 211);
Color32 Color32::DeepPink = Color(255, 20, 147);
Color32 Color32::DeepSkyBlue = Color(0, 191, 255);
Color32 Color32::DimGray = Color(105, 105, 105);
Color32 Color32::DodgerBlue = Color(30, 144, 255);
Color32 Color32::FireBrick = Color(178, 34, 34);
Color32 Color32::FloralWhite = Color(255, 250, 240);
Color32 Color32::ForestGreen = Color(34, 139, 34);
Color32 Color32::Fuchsia = Color(255, 0, 255);
Color32 Color32::Gainsboro = Color(220, 220, 220);
Color32 Color32::GhostWhite = Color(248, 248, 255);
Color32 Color32::Gold = Color(255, 215, 0);
Color32 Color32::Goldenrod = Color(218, 165, 32);
Color32 Color32::Gray = Color(128, 128, 128);
Color32 Color32::Green = Color(0, 128, 0);
Color32 Color32::GreenYellow = Color(173, 255, 47);
Color32 Color32::Honeydew = Color(240, 255, 240);
Color32 Color32::HotPink = Color(255, 105, 180);
Color32 Color32::IndianRed = Color(205, 92, 92);
Color32 Color32::Indigo = Color(75, 0, 130);
Color32 Color32::Ivory = Color(255, 255, 240);
Color32 Color32::Khaki = Color(240, 230, 140);
Color32 Color32::Lavender = Color(230, 230, 250);
Color32 Color32::LavenderBlush = Color(255, 240, 245);
Color32 Color32::LawnGreen = Color(124, 252, 0);
Color32 Color32::LemonChiffon = Color(255, 250, 205);
Color32 Color32::LightBlue = Color(173, 216, 230);
Color32 Color32::LightCoral = Color(240, 128, 128);
Color32 Color32::LightCyan = Color(224, 255, 255);
Color32 Color32::LightGoldenrodYellow = Color(250, 250, 210);
Color32 Color32::LightGray = Color(211, 211, 211);
Color32 Color32::LightGreen = Color(144, 238, 144);
Color32 Color32::LightPink = Color(255, 182, 193);
Color32 Color32::LightSalmon = Color(255, 160, 122);
Color32 Color32::LightSeaGreen = Color(32, 178, 170);
Color32 Color32::LightSkyBlue = Color(135, 206, 250);
Color32 Color32::LightSlateGray = Color(119, 136, 153);
Color32 Color32::LightSteelBlue = Color(176, 196, 222);
Color32 Color32::LightYellow = Color(255, 255, 224);
Color32 Color32::Lime = Color(0, 255, 0);
Color32 Color32::LimeGreen = Color(50, 205, 50);
Color32 Color32::Linen = Color(250, 240, 230);
Color32 Color32::Magenta = Color(255, 0, 255);
Color32 Color32::Maroon = Color(128, 0, 0);
Color32 Color32::MediumAquaMarine = Color(102, 205, 170);
Color32 Color32::MediumBlue = Color(0, 0, 205);
Color32 Color32::MediumOrchid = Color(186, 85, 211);
Color32 Color32::MediumPurple = Color(147, 112, 219);
Color32 Color32::MediumSeaGreen = Color(60, 179, 113);
Color32 Color32::MediumSlateBlue = Color(123, 104, 238);
Color32 Color32::MediumSpringGreen = Color(0, 250, 154);
Color32 Color32::MediumTurquoise = Color(72, 209, 204);
Color32 Color32::MediumVioletRed = Color(199, 21, 133);
Color32 Color32::MidnightBlue = Color(25, 25, 112);
Color32 Color32::MintCream = Color(245, 255, 250);
Color32 Color32::MistyRose = Color(255, 228, 225);
Color32 Color32::Moccasin = Color(255, 228, 181);
Color32 Color32::NavajoWhite = Color(255, 222, 173);
Color32 Color32::Navy = Color(0, 0, 128);
Color32 Color32::Oldlace = Color(253, 245, 230);
Color32 Color32::Olive = Color(128, 128, 0);
Color32 Color32::OliveDrab = Color(107, 142, 35);
Color32 Color32::Orange = Color(255, 165, 0);
Color32 Color32::OrangeRed = Color(255, 69, 0);
Color32 Color32::Orchid = Color(218, 112, 214);
Color32 Color32::PaleAzure = Color(240, 255, 255);
Color32 Color32::PaleGoldenrod = Color(238, 232, 170);
Color32 Color32::PaleGreen = Color(152, 251, 152);
Color32 Color32::PaleTurquoise = Color(175, 238, 238);
Color32 Color32::PaleVioletRed = Color(219, 112, 147);
Color32 Color32::PapayaWhip = Color(255, 239, 213);
Color32 Color32::PeachPuff = Color(255, 218, 185);
Color32 Color32::Peru = Color(205, 133, 63);
Color32 Color32::Pink = Color(255, 192, 203);
Color32 Color32::Plum = Color(221, 160, 221);
Color32 Color32::PowderBlue = Color(176, 224, 230);
Color32 Color32::Purple = Color(128, 0, 128);
Color32 Color32::Red = Color(255, 0, 0);
Color32 Color32::RosyBrown = Color(188, 143, 143);
Color32 Color32::RoyalBlue = Color(65, 105, 225);
Color32 Color32::SaddleBrown = Color(139, 69, 19);
Color32 Color32::Salmon = Color(250, 128, 114);
Color32 Color32::SandyBrown = Color(244, 164, 96);
Color32 Color32::SeaGreen = Color(46, 139, 87);
Color32 Color32::SeaShell = Color(255, 245, 238);
Color32 Color32::Sienna = Color(160, 82, 45);
Color32 Color32::Silver = Color(192, 192, 192);
Color32 Color32::SkyBlue = Color(135, 206, 235);
Color32 Color32::SlateBlue = Color(106, 90, 205);
Color32 Color32::SlateGray = Color(112, 128, 144);
Color32 Color32::Snow = Color(255, 250, 250);
Color32 Color32::SpringGreen = Color(0, 255, 127);
Color32 Color32::SteelBlue = Color(70, 130, 180);
Color32 Color32::Tan = Color(210, 180, 140);
Color32 Color32::Teal = Color(0, 128, 128);
Color32 Color32::Thistle = Color(216, 191, 216);
Color32 Color32::Tomato = Color(255, 99, 71);
Color32 Color32::Turquoise = Color(64, 224, 208);
Color32 Color32::Violet = Color(238, 130, 238);
Color32 Color32::Wheat = Color(245, 222, 179);
Color32 Color32::White = Color(255, 255, 255);
Color32 Color32::WhiteSmoke = Color(245, 245, 245);
Color32 Color32::Yellow = Color(255, 255, 0);
Color32 Color32::YellowGreen = Color(154, 205, 50);
Color32 Color32::Transparent = Color(0, 0, 0, 0);