#include "Shader.h"


Shader* Shader::FromSource(STRING vertPath, STRING fragPath)
{
	ifstream vFile(vertPath);
	ifstream fFile(fragPath);

	string vCode((istreambuf_iterator<char>(vFile)), istreambuf_iterator<char>());
	string fCode((istreambuf_iterator<char>(fFile)), istreambuf_iterator<char>());

	return new Shader(vCode, fCode);
}

Shader* Shader::FromSource(STRING vertPath, STRING geomPath, STRING fragPath)
{
	ifstream vFile(vertPath);
	ifstream gFile(geomPath);
	ifstream fFile(fragPath);

	string vCode((istreambuf_iterator<char>(vFile)), istreambuf_iterator<char>());
	string gCode((istreambuf_iterator<char>(gFile)), istreambuf_iterator<char>());
	string fCode((istreambuf_iterator<char>(fFile)), istreambuf_iterator<char>());

	return new Shader(vCode, gCode, fCode);
}



Shader::Shader(STRING vCode, STRING fCode)
{


	const char* vCstr = vCode.c_str();
	const char* fCstr = fCode.c_str();

	_vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(_vertex, 1, &vCstr, NULL);
	glCompileShader(_vertex);


	int vSuccess;
	char vInfo[512];

	glGetShaderiv(_vertex, GL_COMPILE_STATUS, &vSuccess);
	if (!vSuccess)
	{
		glGetShaderInfoLog(_vertex, 512, NULL, vInfo);
		cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << vInfo << endl;
	};

	_fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(_fragment, 1, &fCstr, NULL);
	glCompileShader(_fragment);

	int fSuccess;
	char fInfo[512];

	glGetShaderiv(_fragment, GL_COMPILE_STATUS, &fSuccess);
	if (!fSuccess)
	{
		glGetShaderInfoLog(_fragment, 512, NULL, fInfo);
		cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << fInfo << endl;
	};

	_programID = glCreateProgram();
	glAttachShader(_programID, _vertex);
	glAttachShader(_programID, _fragment);
	glLinkProgram(_programID);


	glDeleteShader(_vertex);
	glDeleteShader(_fragment);
}

Shader::Shader(STRING vCode, STRING gCode, STRING fCode)
{
	const char* vCstr = vCode.c_str();
	const char* gCstr = gCode.c_str();
	const char* fCstr = fCode.c_str();

	// ---- VERTEX SHADER

	_vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(_vertex, 1, &vCstr, NULL);
	glCompileShader(_vertex);

	int vSuccess;
	char vInfo[512];

	glGetShaderiv(_vertex, GL_COMPILE_STATUS, &vSuccess);
	if (!vSuccess)
	{
		glGetShaderInfoLog(_vertex, 512, NULL, vInfo);
		cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << vInfo << endl;
	}


	// ---- GEOMETRY SHADER 

	_geometry = glCreateShader(GL_GEOMETRY_SHADER);
	glShaderSource(_geometry, 1, &gCstr, NULL);
	glCompileShader(_geometry);

	int gSuccess;
	char gInfo[512];

	glGetShaderiv(_geometry, GL_COMPILE_STATUS, &gSuccess);
	if (!gSuccess)
	{
		glGetShaderInfoLog(_geometry, 512, NULL, gInfo);
		cout << "ERROR::SHADER::GEOMETRY::COMPILATION_FAILED\n" << gInfo << endl;
	}



	// ---- FRAGMENT SHADER

	_fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(_fragment, 1, &fCstr, NULL);
	glCompileShader(_fragment);

	int fSuccess;
	char fInfo[512];

	glGetShaderiv(_fragment, GL_COMPILE_STATUS, &fSuccess);
	if (!fSuccess)
	{
		glGetShaderInfoLog(_fragment, 512, NULL, fInfo);
		cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << fInfo << endl;
	}

	// --------------------

	_programID = glCreateProgram();
	glAttachShader(_programID, _vertex);
	glAttachShader(_programID, _geometry);
	glAttachShader(_programID, _fragment);
	glLinkProgram(_programID);


	int lSuccess;
	char lInfo[512];

	glGetProgramiv(_programID, GL_LINK_STATUS, &lSuccess);
	if (!lSuccess)
	{
		glGetProgramInfoLog(_programID, 512, NULL, lInfo);
		cout << "ERROR::SHADER::LINKING_FAILED\n" << lInfo << endl;
	}
	// ------------------------

	glDeleteShader(_vertex);
	glDeleteShader(_geometry);
	glDeleteShader(_fragment);
}
void Shader::UseProgram()
{
	glUseProgram(_programID);
}
void Shader::SetMatrix(const string &name, fMatrix4 val)
{
	glUniformMatrix4fv(glGetUniformLocation(_programID, name.c_str()), 1, GL_FALSE, &val[0][0]);
}

void Shader::SetMatrix(const string& name, fMatrix3 val)
{
	glUniformMatrix3fv(glGetUniformLocation(_programID, name.c_str()), 1, GL_FALSE, &val[0][0]);
}

void Shader::SetFloat(const string &name, float val)
{
	glUniform1f(glGetUniformLocation(_programID, name.c_str()), val);
}
void Shader::SetInt(const string &name, int val)
{
	glUniform1i(glGetUniformLocation(_programID, name.c_str()), val);
}

void Shader::SetVector(const string& name, fVector2 val)
{
	glUniform2fv(glGetUniformLocation(_programID, name.c_str()), 1, &(val.x));
}
void Shader::SetVector(const string& name, fVector3 val)
{
	glUniform3fv(glGetUniformLocation(_programID, name.c_str()), 1, &(val.x));
}
void Shader::SetVector(const string& name, fVector4 val)
{
	glUniform4fv(glGetUniformLocation(_programID, name.c_str()), 1, &(val.x));
}
void Shader::SetColor(const string& name, Color val)
{
	glUniform4fv(glGetUniformLocation(_programID, name.c_str()), 1, &(val.r));
}
void Shader::SetTexture(_UNS_ FL_INT32 _id, Texture* _tex)
{
	glActiveTexture(GL_TEXTURE0 + _id);
	glBindTexture(GL_TEXTURE_2D, _tex->_handle);
}

void Shader::SetTexture(const string& name, Texture* _tex)
{
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, _tex->_handle);
}