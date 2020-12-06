#pragma once

#include "../../dll/nchfx.h"


EXPORT(class, Query)
{
public:
	Query(GLenum type);

	void Begin();
	void End();

	template<typename T>
	T GetResult();


	template<>
	int GetResult<int>();


	bool IsComplete();


private:
	GLenum target;
	GLuint handle;
};

