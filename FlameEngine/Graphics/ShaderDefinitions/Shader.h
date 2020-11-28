#pragma once


#include "../../dll/nchfx.h"
#include "../../Graphics/Texture.h"
#include "../../Graphics/MultisampleTexture.h"
#include "../../Graphics/Color.h"

using namespace std;

EXPORT_CLASS Shader
{
public:

	GLuint _vertex;
	GLuint _fragment;
	GLuint _geometry;

	GLuint _programID;

	Shader(STRING vertSource, STRING fragSource);
	Shader(STRING vertSource, STRING geomSource, STRING fragSource);
	~Shader();


	static Shader* FromSource(STRING vertPath, STRING fragPath);
	static Shader* FromSource(STRING vertPath, STRING geomPath, STRING fragPath);


	void UseProgram();
	void SetMatrix(const string& name, fMatrix3 val);
	void SetMatrix(const string& name, fMatrix4 val);

	void SetVector(const string& name, fVector2 val);
	void SetVector(const string& name, fVector3 val);
	void SetVector(const string& name, fVector4 val);

	void SetColor(const string& name, Color color);

	void SetFloat(const string& name, float val);
	void SetInt(const string& name, int val);

	void SetTexture(_UNS_ FL_INT32 _id, Texture* _texture);
	void SetTexture(_UNS_ FL_INT32 _id, MultisampleTexture* _texture);
	void SetTexture(const string& name, Texture* _texture);
	void SetTexture(const string& name, MultisampleTexture* _texture);
};

