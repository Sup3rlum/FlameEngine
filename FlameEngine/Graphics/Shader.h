#pragma once


#include "../dll/nchfx.h"
#include "../Graphics/Texture.h"
#include "../Graphics/Color.h"

using namespace std;
using namespace glm;

EXPORT_CLASS Shader
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
	void SetMatrix(const string& name, Matrix3 val);
	void SetMatrix(const string& name, Matrix4 val);

	void SetVector(const string& name, Vector2 val);
	void SetVector(const string& name, Vector3 val);
	void SetVector(const string& name, Vector4 val);

	void SetColor(const string& name, Color color);

	void SetFloat(const string& name, float val);
	void SetInt(const string& name, int val);

	void SetTexture(_UNS_ FL_INT32 _id, Texture* _texture);
};

