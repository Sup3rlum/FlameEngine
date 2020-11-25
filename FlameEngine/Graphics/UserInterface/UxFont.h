#pragma once

#include  "../Texture.h"
#include "../../Mathematics/Module.h"

#include <FTGL/ftgl.h>

EXPORT_STRUCT UxCharacter
{
	Texture * texture;   // ID handle of the glyph texture
	fVector2 Size;    // Size of glyph
	fVector2 Bearing;  // Offset from baseline to left/top of glyph
	GLuint Advance;    // Horizontal offset to advance to next glyph
};



EXPORT_CLASS UxFont
{
public:
	UxFont(STRING name, int fontsize);

	int FontSize;

	std::map<GLchar, UxCharacter> Characters;
};

