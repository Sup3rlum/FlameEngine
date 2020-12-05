#pragma once


#include "../../dll/nchfx.h"
#include "../Common/Texture.h"
#include "../Common/MultisampleTexture.h"
#include "../Common/Color.h"

using namespace std;

EXPORT(class,  Shader)
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
	void SetMatrix(const string& name, FMatrix3 val);
	void SetMatrix(const string& name, FMatrix4 val);

	void SetVector(const string& name, FVector2 val);
	void SetVector(const string& name, FVector3 val);
	void SetVector(const string& name, FVector4 val);

	void SetColor(const string& name, Color color);

	void SetFloat(const string& name, float val);
	void SetInt(const string& name, int val);

	void SetTexture(_UNS_ FL_INT32 _id, Texture* _texture);
	void SetTexture(_UNS_ FL_INT32 _id, MultisampleTexture* _texture);
	void SetTexture(const string& name, Texture* _texture);
	void SetTexture(const string& name, MultisampleTexture* _texture);
};

