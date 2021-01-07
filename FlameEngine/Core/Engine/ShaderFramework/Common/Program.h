#pragma once


#include "Core/Common/CoreCommon.h"
#include "Core/Engine/RenderingSystem/Common/Texture/Texture.h"
#include "Core/Engine/RenderingSystem/Common/Color.h"

#include "Shader.h"
#include "../CompilationServices/FLSLCompilerService.h"


EXPORT(class,  Program)
{
	uint32_t _programID;

public:


	template<size_t _Size>
	Program(Shader* (&shaders)[_Size])
	{
		assert(_Size <= 6);


		_programID = glCreateProgram();

		for (int i = 0; i < _Size; i++)
		{
			glAttachShader(_programID, shaders[i]->handle);
		}
		glLinkProgram(_programID);

		int lSuccess;
		char lInfo[512];

		glGetProgramiv(_programID, GL_LINK_STATUS, &lSuccess);
		if (!lSuccess)
		{
			glGetProgramInfoLog(_programID, 512, NULL, lInfo);
			FLAME_ERROR("ERROR::SHADER::LINKING_FAILED\n" + STRING(lInfo));
		}


	}
	~Program();



	void UseProgram();

	template<typename U>
	void SetUniform(STRING name, U value);

	template<>
	void SetUniform<FMatrix2>(STRING name, FMatrix2 val);

	template<>
	void SetUniform<FMatrix3>(STRING name, FMatrix3 val);

	template<>
	void SetUniform<FMatrix4>(STRING name, FMatrix4 val);



	template<>
	void SetUniform<FVector2>(STRING name, FVector2 val);

	template<>
	void SetUniform<FVector3>(STRING name, FVector3 val);

	template<>
	void SetUniform<FVector4>(STRING name, FVector4 val);



	template<>
	void SetUniform<Color>(STRING name, Color val);

	template<>
	void SetUniform<float>(STRING name, float val);

	template<>
	void SetUniform<int>(STRING name, int val);


	void SetTexture(_UNS_ FL_INT32 _id, Texture* _texture);
	void SetTexture(_UNS_ FL_INT32 _id, MultisampleTexture* _texture);
	void SetTexture(STRING name, Texture* _texture);
	void SetTexture(STRING name, MultisampleTexture* _texture);
};

