#pragma once

#include "nchfx.h"
#include <ft2build.h>
#include FT_FREETYPE_H

#include "Texture.h"

struct Character {
	Texture* texture;   // ID handle of the glyph texture
	glm::ivec2 Size;    // Size of glyph
	glm::ivec2 Bearing;  // Offset from baseline to left/top of glyph
	GLuint Advance;    // Horizontal offset to advance to next glyph
};


class Font
{
public:
	Font(const char* _fontFamily, int size);

	std::map<GLchar, Character> Characters;
};

