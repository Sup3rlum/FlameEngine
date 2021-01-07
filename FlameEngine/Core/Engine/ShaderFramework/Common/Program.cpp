#include "Program.h"



void Program::UseProgram()
{
	glUseProgram(_programID);
}





template<>
void Program::SetUniform<FMatrix4>(STRING name, FMatrix4 val)
{
	glUniformMatrix4fv(glGetUniformLocation(_programID, name.c_str()), 1, GL_FALSE, &val[0][0]);
}


template<>
void Program::SetUniform<FMatrix3>(STRING name, FMatrix3 val)
{
	glUniformMatrix3fv(glGetUniformLocation(_programID, name.c_str()), 1, GL_FALSE, &val[0][0]);
}

template<>
void Program::SetUniform<FMatrix2>(STRING name, FMatrix2 val)
{
	glUniformMatrix2fv(glGetUniformLocation(_programID, name.c_str()), 1, GL_FALSE, &val[0][0]);
}




template<>
void Program::SetUniform<float>(STRING name, float val)
{
	glUniform1f(glGetUniformLocation(_programID, name.c_str()), val);
}
template<>
void Program::SetUniform<int>(STRING name, int val)
{
	glUniform1i(glGetUniformLocation(_programID, name.c_str()), val);
}



template<>
void Program::SetUniform<FVector4>(STRING name, FVector4 val)
{
	glUniform4fv(glGetUniformLocation(_programID, name.c_str()), 1, &(val.x));
}
template<>
void Program::SetUniform<FVector2>(STRING name, FVector2 val)
{
	glUniform2fv(glGetUniformLocation(_programID, name.c_str()), 1, &(val.x));
}
template<>
void Program::SetUniform<FVector3>(STRING name, FVector3 val)
{
	glUniform3fv(glGetUniformLocation(_programID, name.c_str()), 1, &(val.x));
}
template<>
void Program::SetUniform<Color>(STRING name, Color val)
{
	glUniform4fv(glGetUniformLocation(_programID, name.c_str()), 1, &(val.r));
}




void Program::SetTexture(_UNS_ FL_INT32 _id, Texture* _tex)
{
	glActiveTexture(GL_TEXTURE0 + _id);
	glBindTexture(GL_TEXTURE_2D, _tex->GetHandle());
}
void Program::SetTexture(_UNS_ FL_INT32 _id, MultisampleTexture* _tex)
{
	glActiveTexture(GL_TEXTURE0 + _id);
	glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, _tex->GetHandle());
}




void Program::SetTexture(STRING name, Texture* _tex)
{
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, _tex->GetHandle());
}
void Program::SetTexture(STRING name, MultisampleTexture* _tex)
{
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, _tex->GetHandle());
}