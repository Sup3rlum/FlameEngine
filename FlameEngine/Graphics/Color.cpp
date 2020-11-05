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

Color Color::aliceblue				= Color(240, 248, 255);
Color Color::antiquewhite			= Color(250, 235, 215);
Color Color::aqua					= Color(0, 255, 255);
Color Color::aquamarine				= Color(127, 255, 212);
Color Color::azure					= Color(240, 255, 255);
Color Color::beige					= Color(245, 245, 220);
Color Color::bisque					= Color(255, 228, 196);
Color Color::black					= Color(0, 0, 0);
Color Color::blanchedalmond			= Color(255, 235, 205);
Color Color::blue					= Color(0, 0, 255);
Color Color::blueviolet				= Color(138, 43, 226);
Color Color::brown					= Color(165, 42, 42);
Color Color::burlywood				= Color(222, 184, 135);
Color Color::cadetblue				= Color(95, 158, 160);
Color Color::chartreuse				= Color(127, 255, 0);
Color Color::chocolate				= Color(210, 105, 30);
Color Color::coral					= Color(255, 127, 80);
Color Color::cornflowerblue			= Color(100, 149, 237);
Color Color::cornsilk				= Color(255, 248, 220);
Color Color::crimson				= Color(220, 20, 60);
Color Color::cyan					= Color(0, 255, 255);
Color Color::darkblue				= Color(0, 0, 139);
Color Color::darkcyan				= Color(0, 139, 139);
Color Color::darkgoldenrod			= Color(184, 134, 11);
Color Color::darkgray				= Color(169, 169, 169);
Color Color::darkgreen				= Color(0, 100, 0);
Color Color::darkkhaki				= Color(189, 183, 107);
Color Color::darkmagenta			= Color(139, 0, 139);
Color Color::darkolivegreen			= Color(85, 107, 47);
Color Color::darkorange				= Color(255, 140, 0);
Color Color::darkorchid				= Color(153, 50, 204);
Color Color::darkred				= Color(139, 0, 0);
Color Color::darksalmon				= Color(233, 150, 122);
Color Color::darkseagreen			= Color(143, 188, 139);
Color Color::darkslateblue			= Color(72, 61, 139);
Color Color::darkslategray			= Color(47, 79, 79);
Color Color::darkturquoise			= Color(0, 206, 209);
Color Color::darkviolet				= Color(148, 0, 211);
Color Color::deeppink				= Color(255, 20, 147);
Color Color::deepskyblue			= Color(0, 191, 255);
Color Color::dimgray				= Color(105, 105, 105);
Color Color::dodgerblue				= Color(30, 144, 255);
Color Color::firebrick				= Color(178, 34, 34);
Color Color::floralwhite			= Color(255, 250, 240);
Color Color::forestgreen			= Color(34, 139, 34);
Color Color::fuchsia				= Color(255, 0, 255);
Color Color::gainsboro				= Color(220, 220, 220);
Color Color::ghostwhite				= Color(248, 248, 255);
Color Color::gold					= Color(255, 215, 0);
Color Color::goldenrod				= Color(218, 165, 32);
Color Color::gray					= Color(128, 128, 128);
Color Color::green					= Color(0, 128, 0);
Color Color::greenyellow			= Color(173, 255, 47);
Color Color::honeydew				= Color(240, 255, 240);
Color Color::hotpink				= Color(255, 105, 180);
Color Color::indianred				= Color(205, 92, 92);
Color Color::indigo					= Color(75, 0, 130);
Color Color::ivory					= Color(255, 255, 240);
Color Color::khaki					= Color(240, 230, 140);
Color Color::lavender				= Color(230, 230, 250);
Color Color::lavenderblush			= Color(255, 240, 245);
Color Color::lawngreen				= Color(124, 252, 0);
Color Color::lemonchiffon			= Color(255, 250, 205);
Color Color::lightblue				= Color(173, 216, 230);
Color Color::lightcoral				= Color(240, 128, 128);
Color Color::lightcyan				= Color(224, 255, 255);
Color Color::lightgoldenrodyellow	= Color(250, 250, 210);
Color Color::lightgray				= Color(211, 211, 211);
Color Color::lightgreen				= Color(144, 238, 144);
Color Color::lightpink				= Color(255, 182, 193);
Color Color::lightsalmon			= Color(255, 160, 122);
Color Color::lightseagreen			= Color(32, 178, 170);
Color Color::lightskyblue			= Color(135, 206, 250);
Color Color::lightslategray			= Color(119, 136, 153);
Color Color::lightsteelblue			= Color(176, 196, 222);
Color Color::lightyellow			= Color(255, 255, 224);
Color Color::lime					= Color(0, 255, 0);
Color Color::limegreen				= Color(50, 205, 50);
Color Color::linen					= Color(250, 240, 230);
Color Color::magenta				= Color(255, 0, 255);
Color Color::maroon					= Color(128, 0, 0);
Color Color::mediumaquamarine		= Color(102, 205, 170);
Color Color::mediumblue				= Color(0, 0, 205);
Color Color::mediumorchid			= Color(186, 85, 211);
Color Color::mediumpurple			= Color(147, 112, 219);
Color Color::mediumseagreen			= Color(60, 179, 113);
Color Color::mediumslateblue		= Color(123, 104, 238);
Color Color::mediumspringgreen		= Color(0, 250, 154);
Color Color::mediumturquoise		= Color(72, 209, 204);
Color Color::mediumvioletred		= Color(199, 21, 133);
Color Color::midnightblue			= Color(25, 25, 112);
Color Color::mintcream				= Color(245, 255, 250);
Color Color::mistyrose				= Color(255, 228, 225);
Color Color::moccasin				= Color(255, 228, 181);
Color Color::navajowhite			= Color(255, 222, 173);
Color Color::navy					= Color(0, 0, 128);
Color Color::oldlace				= Color(253, 245, 230);
Color Color::olive					= Color(128, 128, 0);
Color Color::olivedrab				= Color(107, 142, 35);
Color Color::orange					= Color(255, 165, 0);
Color Color::orangered				= Color(255, 69, 0);
Color Color::orchid					= Color(218, 112, 214);
Color Color::palegoldenrod			= Color(238, 232, 170);
Color Color::palegreen				= Color(152, 251, 152);
Color Color::paleturquoise			= Color(175, 238, 238);
Color Color::palevioletred			= Color(219, 112, 147);
Color Color::papayawhip				= Color(255, 239, 213);
Color Color::peachpuff				= Color(255, 218, 185);
Color Color::peru					= Color(205, 133, 63);
Color Color::pink					= Color(255, 192, 203);
Color Color::plum					= Color(221, 160, 221);
Color Color::powderblue				= Color(176, 224, 230);
Color Color::purple					= Color(128, 0, 128);
Color Color::red					= Color(255, 0, 0);
Color Color::rosybrown				= Color(188, 143, 143);
Color Color::royalblue				= Color(65, 105, 225);
Color Color::saddlebrown			= Color(139, 69, 19);
Color Color::salmon					= Color(250, 128, 114);
Color Color::sandybrown				= Color(244, 164, 96);
Color Color::seagreen				= Color(46, 139, 87);
Color Color::seashell				= Color(255, 245, 238);
Color Color::sienna					= Color(160, 82, 45);
Color Color::silver					= Color(192, 192, 192);
Color Color::skyblue				= Color(135, 206, 235);
Color Color::slateblue				= Color(106, 90, 205);
Color Color::slategray				= Color(112, 128, 144);
Color Color::snow					= Color(255, 250, 250);
Color Color::springgreen			= Color(0, 255, 127);
Color Color::steelblue				= Color(70, 130, 180);
Color Color::tan					= Color(210, 180, 140);
Color Color::teal					= Color(0, 128, 128);
Color Color::thistle				= Color(216, 191, 216);
Color Color::tomato					= Color(255, 99, 71);
Color Color::turquoise				= Color(64, 224, 208);
Color Color::violet					= Color(238, 130, 238);
Color Color::wheat					= Color(245, 222, 179);
Color Color::white					= Color(255, 255, 255);
Color Color::whitesmoke				= Color(245, 245, 245);
Color Color::yellow					= Color(255, 255, 0);
Color Color::yellowgreen			= Color(154, 205, 50);