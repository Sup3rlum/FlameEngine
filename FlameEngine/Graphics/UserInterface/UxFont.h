#pragma once

#include  "../Common/Texture.h"
#include "../../Mathematics/Module.h"

#include <FTGL/ftgl.h>

EXPORT(struct, UxCharacter)
{
	Texture * texture;   // ID handle of the glyph texture
	fVector2 Size;    // Size of glyph
	fVector2 Bearing;  // Offset from baseline to left/top of glyph
	GLuint Advance;    // Horizontal offset to advance to next glyph

	UxCharacter() : texture(NULL), Size(fVector2(0)), Bearing(fVector2(0)), Advance(0) {}
	UxCharacter(Texture * t, fVector2 s, fVector2 b, GLuint a) : texture(t), Size(s), Bearing(b), Advance(a) {}
};



EXPORT(class,  UxFont)
{
public:
	UxFont(STRING name, int fontsize);

	int FontSize;

	std::map<GLchar, UxCharacter> Characters;
};

