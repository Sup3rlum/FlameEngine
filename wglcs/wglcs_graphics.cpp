#include "wglcs_graphics.h"


extern "C"
{
	WGLCS_EXPORT unsigned int wglcsCreateShader(unsigned int type, char* string)
	{
		GLuint _vertex = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(_vertex, 1, &string, NULL);
		glCompileShader(_vertex);


		int vSuccess;
		char vInfo[512];

		glGetShaderiv(_vertex, GL_COMPILE_STATUS, &vSuccess);
		if (!vSuccess)
		{
			glGetShaderInfoLog(_vertex, 512, NULL, vInfo);
			//cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << vInfo << endl;
		};

		return _vertex;
	}
	WGLCS_EXPORT unsigned int wglcsCreateProgram(unsigned int _vertex, unsigned int _fragment)
	{
		GLuint _programID = glCreateProgram();
		glAttachShader(_programID, _vertex);
		glAttachShader(_programID, _fragment);
		glLinkProgram(_programID);


		glDeleteShader(_vertex);
		glDeleteShader(_fragment);

		return _programID;
	}
	WGLCS_EXPORT void wglcsUniformFloat(unsigned int _program, char* name, int _uniformSize, float* data)
	{
		GLint _uloc = glGetUniformLocation(_program, name);

		if (_uniformSize == 2)
		{
			glUniform2fv(_uloc, 1, data);
		}
		if (_uniformSize == 3)
		{
			glUniform3fv(_uloc, 1, data);
		}
		if (_uniformSize == 4)
		{
			glUniform4fv(_uloc, 1, data);
		}
	}
	WGLCS_EXPORT void wglcsUniformMatrix(unsigned int _program, char* name, float* data)
	{
		GLint _uloc = glGetUniformLocation(_program, name);

		glUniformMatrix4fv(_uloc, 1, GL_FALSE, data);

	}
}