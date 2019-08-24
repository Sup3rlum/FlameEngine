#pragma once

#include "nchfx.h"
#include "Texture.h"
#include "ContextDescription.h"
#include "Shader.h"
#include "VertexBuffer.h"
#include "Vertex.h"
#include "Font.h"

using namespace glm;
using namespace std;

class DELUSION_DLL RenderBatch
{

public:
	RenderBatch(ContextDescription* _context);

	void DrawTexture(Texture* _tex, float x, float y, float width, float height);
	void DrawTextures(int count, Texture** _tex, float x, float y, float width, float height);
	void DrawTextures(int count, Texture** _tex, float x, float y, float width, float height, Matrix4 _m, Shader* _shader);

	void DrawTexture(GLuint _tex, float x, float y, float width, float height);

	void DrawString(string text, Font* font, float x, float y, Color color);


	Shader* _shader;
	Shader* _shaderString;
	VertexBuffer* _vb;

	Matrix4 View;
};

