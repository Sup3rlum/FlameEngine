#pragma once

#include "../../dll/nchfx.h"
#include "Texture.h"
#include "MultisampleTexture.h"
#include "../ShaderDefinitions/Shader.h"
#include "VertexBuffer.h"
#include "Vertex.h"
#include "../../Graphics/UserInterface/UxFont.h"
#include "RenderState.h"
#include "../../Context/ContextDescription.h"
#include "../../Context/Context.h"


using namespace std;

EXPORT(class,  RenderBatch)
{

public:
	RenderBatch(Context*_context);

	void DrawTexture(Texture* _tex, float x, float y, float width, float height);
	void DrawMultisampleTexture(MultisampleTexture* _tex, float x, float y, float width, float height);
	void DrawTexture(Texture* _tex, float x, float y, float width, float height, Shader* _shader);
	void DrawTextures(int count, Texture** _tex, float x, float y, float width, float height);
	void DrawTextures(int count, Texture** _tex, float x, float y, float width, float height, Shader* _shader);
	void DrawTextures(int count, Texture** _tex, float x, float y, float width, float height, FMatrix4 _m, Shader* _shader);

	void DrawString(string text, UxFont* font, float x, float y, Color color);


	Shader* _shader;
	Shader* _msShader;
	Shader* _shaderString;
	VertexBuffer* _vb;

	FMatrix4 View;

	RenderState* State;

};

