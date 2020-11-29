#include "Color.h"

Color::Color(BYTE r, BYTE g, BYTE b)
{
	this->r = (float)r / 255.0f;
	this->g = (float)g / 255.0f;
	this->b = (float)b / 255.0f;
	this->a = 1.0f;
}

Color::Color(BYTE r, BYTE g, BYTE b, BYTE a)
{
	this->r = (float)r / 255.0f;
	this->g = (float)g / 255.0f;
	this->b = (float)b / 255.0f;
	this->a = (float)a / 255.0f;
}
Color::Color(_UNS_ FL_INT32 hex)
{
	Color(
		((hex >> 24) & 0xFF),
		((hex >> 16) & 0xFF),
		((hex >> 8) & 0xFF),
		(hex & 0xFF)
	);
}

Color::Color()
{

}

Color Color::Alicelue				= Color(240, 248, 255);
Color Color::AntiqueWhite			= Color(250, 235, 215);
Color Color::Aqua					= Color(0, 255, 255);
Color Color::Aquamarine				= Color(127, 255, 212);
Color Color::Azure					= Color(240, 255, 255);
Color Color::Beige					= Color(245, 245, 220);
Color Color::Bisque					= Color(255, 228, 196);
Color Color::Black					= Color(0, 0, 0);
Color Color::BlanchedAlmond			= Color(255, 235, 205);
Color Color::Blue					= Color(0, 0, 255);
Color Color::Blueviolet				= Color(138, 43, 226);
Color Color::Brown					= Color(165, 42, 42);
Color Color::Burlywood				= Color(222, 184, 135);
Color Color::CadetBlue				= Color(95, 158, 160);
Color Color::Chartreuse				= Color(127, 255, 0);
Color Color::Chocolate				= Color(210, 105, 30);
Color Color::Coral					= Color(255, 127, 80);
Color Color::CornflowerBlue			= Color(100, 149, 237);
Color Color::Cornsilk				= Color(255, 248, 220);
Color Color::Crimson				= Color(220, 20, 60);
Color Color::Cyan					= Color(0, 255, 255);
Color Color::DarkBlue				= Color(0, 0, 139);
Color Color::DarkCyan				= Color(0, 139, 139);
Color Color::DarkGoldenrod			= Color(184, 134, 11);
Color Color::DarkGray				= Color(169, 169, 169);
Color Color::DarkGreen				= Color(0, 100, 0);
Color Color::DarkKhaki				= Color(189, 183, 107);
Color Color::DarkMagenta			= Color(139, 0, 139);
Color Color::DarkOliveGreen			= Color(85, 107, 47);
Color Color::DarkOrange				= Color(255, 140, 0);
Color Color::DarkOrchid				= Color(153, 50, 204);
Color Color::DarkRed				= Color(139, 0, 0);
Color Color::DarkSalmon				= Color(233, 150, 122);
Color Color::DarkSeaGreen			= Color(143, 188, 139);
Color Color::DarkSlateBlue			= Color(72, 61, 139);
Color Color::DarkSlateGray			= Color(47, 79, 79);
Color Color::DarkTurquoise			= Color(0, 206, 209);
Color Color::DarkViolet				= Color(148, 0, 211);
Color Color::DeepPink				= Color(255, 20, 147);
Color Color::DeepSkyBlue			= Color(0, 191, 255);
Color Color::DimGray				= Color(105, 105, 105);
Color Color::DodgerBlue				= Color(30, 144, 255);
Color Color::FireBrick				= Color(178, 34, 34);
Color Color::FloralWhite			= Color(255, 250, 240);
Color Color::ForestGreen			= Color(34, 139, 34);
Color Color::Fuchsia				= Color(255, 0, 255);
Color Color::Gainsboro				= Color(220, 220, 220);
Color Color::GhostWhite				= Color(248, 248, 255);
Color Color::Gold					= Color(255, 215, 0);
Color Color::Goldenrod				= Color(218, 165, 32);
Color Color::Gray					= Color(128, 128, 128);
Color Color::Green					= Color(0, 128, 0);
Color Color::GreenYellow			= Color(173, 255, 47);
Color Color::Honeydew				= Color(240, 255, 240);
Color Color::HotPink				= Color(255, 105, 180);
Color Color::IndianRed				= Color(205, 92, 92);
Color Color::Indigo					= Color(75, 0, 130);
Color Color::Ivory					= Color(255, 255, 240);
Color Color::Khaki					= Color(240, 230, 140);
Color Color::Lavender				= Color(230, 230, 250);
Color Color::LavenderBlush			= Color(255, 240, 245);
Color Color::LawnGreen				= Color(124, 252, 0);
Color Color::LemonChiffon			= Color(255, 250, 205);
Color Color::LightBlue				= Color(173, 216, 230);
Color Color::LightCoral				= Color(240, 128, 128);
Color Color::LightCyan				= Color(224, 255, 255);
Color Color::LightGoldenrodYellow	= Color(250, 250, 210);
Color Color::LightGray				= Color(211, 211, 211);
Color Color::LightGreen				= Color(144, 238, 144);
Color Color::LightPink				= Color(255, 182, 193);
Color Color::LightSalmon			= Color(255, 160, 122);
Color Color::LightSeaGreen			= Color(32, 178, 170);
Color Color::LightSkyBlue			= Color(135, 206, 250);
Color Color::LightSlateGray			= Color(119, 136, 153);
Color Color::LightSteelBlue			= Color(176, 196, 222);
Color Color::LightYellow			= Color(255, 255, 224);
Color Color::Lime					= Color(0, 255, 0);
Color Color::LimeGreen				= Color(50, 205, 50);
Color Color::Linen					= Color(250, 240, 230);
Color Color::Magenta				= Color(255, 0, 255);
Color Color::Maroon					= Color(128, 0, 0);
Color Color::MediumAquaMarine		= Color(102, 205, 170);
Color Color::MediumBlue				= Color(0, 0, 205);
Color Color::MediumOrchid			= Color(186, 85, 211);
Color Color::MediumPurple			= Color(147, 112, 219);
Color Color::MediumSeaGreen			= Color(60, 179, 113);
Color Color::MediumSlateBlue		= Color(123, 104, 238);
Color Color::MediumSpringGreen		= Color(0, 250, 154);
Color Color::MediumTurquoise		= Color(72, 209, 204);
Color Color::MediumVioletRed		= Color(199, 21, 133);
Color Color::MidnightBlue			= Color(25, 25, 112);
Color Color::MintCream				= Color(245, 255, 250);
Color Color::MistyRose				= Color(255, 228, 225);
Color Color::Moccasin				= Color(255, 228, 181);
Color Color::NavajoWhite			= Color(255, 222, 173);
Color Color::Navy					= Color(0, 0, 128);
Color Color::Oldlace				= Color(253, 245, 230);
Color Color::Olive					= Color(128, 128, 0);
Color Color::OliveDrab				= Color(107, 142, 35);
Color Color::Orange					= Color(255, 165, 0);
Color Color::OrangeRed				= Color(255, 69, 0);
Color Color::Orchid					= Color(218, 112, 214);
Color Color::PaleGoldenrod			= Color(238, 232, 170);
Color Color::PaleGreen				= Color(152, 251, 152);
Color Color::PaleTurquoise			= Color(175, 238, 238);
Color Color::PaleVioletRed			= Color(219, 112, 147);
Color Color::PapayaWhip				= Color(255, 239, 213);
Color Color::PeachPuff				= Color(255, 218, 185);
Color Color::Peru					= Color(205, 133, 63);
Color Color::Pink					= Color(255, 192, 203);
Color Color::Plum					= Color(221, 160, 221);
Color Color::PowderBlue				= Color(176, 224, 230);
Color Color::Purple					= Color(128, 0, 128);
Color Color::Red					= Color(255, 0, 0);
Color Color::RosyBrown				= Color(188, 143, 143);
Color Color::RoyalBlue				= Color(65, 105, 225);
Color Color::SaddleBrown			= Color(139, 69, 19);
Color Color::Salmon					= Color(250, 128, 114);
Color Color::SandyBrown				= Color(244, 164, 96);
Color Color::SeaGreen				= Color(46, 139, 87);
Color Color::SeaShell				= Color(255, 245, 238);
Color Color::Sienna					= Color(160, 82, 45);
Color Color::Silver					= Color(192, 192, 192);
Color Color::SkyBlue				= Color(135, 206, 235);
Color Color::SlateBlue				= Color(106, 90, 205);
Color Color::SlateGray				= Color(112, 128, 144);
Color Color::Snow					= Color(255, 250, 250);
Color Color::SpringGreen			= Color(0, 255, 127);
Color Color::SteelBlue				= Color(70, 130, 180);
Color Color::Tan					= Color(210, 180, 140);
Color Color::Teal					= Color(0, 128, 128);
Color Color::Thistle				= Color(216, 191, 216);
Color Color::Tomato					= Color(255, 99, 71);
Color Color::Turquoise				= Color(64, 224, 208);
Color Color::Violet					= Color(238, 130, 238);
Color Color::Wheat					= Color(245, 222, 179);
Color Color::White					= Color(255, 255, 255);
Color Color::WhiteSmoke				= Color(245, 245, 245);
Color Color::Yellow					= Color(255, 255, 0);
Color Color::YellowGreen			= Color(154, 205, 50);
Color Color::Transparent			= Color(0, 0, 0, 0);