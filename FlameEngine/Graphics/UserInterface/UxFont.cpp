#include "UxFont.h"


UxFont::UxFont(STRING _fontName, int _fontSize)
{

	FontSize = _fontSize;

	FT_Library ft;
	if (FT_Init_FreeType(&ft))
		std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;

	FT_Face face;
	if (FT_New_Face(ft, _fontName.c_str(), 0, &face))
		std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;

	FT_Set_Pixel_Sizes(face, 0, _fontSize);

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	for (GLubyte c = 0; c < 128; c++)
	{
		if (FT_Load_Char(face, c, FT_LOAD_RENDER))
		{
			std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
			continue;
		}


		Texture* _texture = new Texture(
			face->glyph->bitmap.width,
			face->glyph->bitmap.rows,
			GL_RED,
			GL_RED,
			GL_UNSIGNED_BYTE,
			false
		);

		_texture->SetData(face->glyph->bitmap.buffer);

		_texture->SetParameter(GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		_texture->SetParameter(GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		_texture->SetParameter(GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		_texture->SetParameter(GL_TEXTURE_MAG_FILTER, GL_LINEAR);


		// Now store character for later use
		UxCharacter character = {
			_texture,
			FVector2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
			FVector2(face->glyph->bitmap_left, face->glyph->bitmap_top),
			(unsigned int)face->glyph->advance.x
		};
		Characters.insert(std::pair<GLchar, UxCharacter>(c, character));
	}

	// Destroy FreeType once we're finished
	FT_Done_Face(face);
	FT_Done_FreeType(ft);
}