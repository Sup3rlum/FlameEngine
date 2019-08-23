#pragma once


#include "nchfx.h"
#include "Texture.h"

using namespace std;
using namespace glm;

class DELUSION_DLL Shader
{
public:

	GLuint _vertex;
	GLuint _fragment;
	GLuint _geometry;

	GLuint _programID;

	Shader(const char* vertPath, const char* fragPath);
	Shader(const char* vertPath, const char* geomPath, const char* fragPath);
	~Shader();

	void UseProgram();
	void SetMatrix(const string& name, mat4x4 val);

	void SetVector(const string& name, vec2 val);
	void SetVector(const string& name, vec3 val);
	void SetVector(const string& name, vec4 val);

	void SetFloat(const string& name, float val);
	void SetInt(const string& name, int val);

	void SetTexture(GLuint _id, Texture* _texture);
	void SetTexture(GLuint _id, GLuint tex);
};

