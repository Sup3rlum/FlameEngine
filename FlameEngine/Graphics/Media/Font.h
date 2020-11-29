#pragma once

#include "../../dll/nchfx.h"
#include <ft2build.h>
#include FT_FREETYPE_H

#include "../../Graphics/Common/Texture.h"
#include "../../Mathematics/Module.h"

EXPORT(struct,  Character)
{
	Texture* texture;   // ID handle of the glyph texture
	fVector2 Size;    // Size of glyph
	fVector2 Bearing;  // Offset from baseline to left/top of glyph
	GLuint Advance;    // Horizontal offset to advance to next glyph
};


EXPORT(class,  Font)
{
public:
	Font(const char* _fontFamily, int size);

	std::map<GLchar, Character> Characters;
};

