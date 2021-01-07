#pragma once

#include "../../dll/nchfx.h"
#include "../Common/Texture.h"
#include "../Common/MultisampleTexture.h"
#include "../Common/Color.h"


EXPORT(enum class, ShaderType)
{
	VERTEX = GL_VERTEX_SHADER,
	GEOMETRY = GL_GEOMETRY_SHADER,
	FRAGMENT = GL_FRAGMENT_SHADER,
	TESSELATION_CONTROL = GL_TESS_CONTROL_SHADER,
	TESSELATION_EVALUATION = GL_TESS_EVALUATION_SHADER,
	COMPUTE = GL_COMPUTE_SHADER
};



EXPORT(class, Shader)
{
public:
	Shader(ShaderType shadertype) : type(shadertype) {}
	~Shader();

	GLuint handle;

	ShaderType type;
};

