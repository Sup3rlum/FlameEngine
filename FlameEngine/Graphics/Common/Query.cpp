#include "Query.h"


Query::Query(GLenum type)
{
	target = type;

	glGenQueries(1, &handle);
}


void Query::Begin()
{
	glBeginQuery(target, handle);
}
void Query::End()
{
	glEndQuery(target);
}

template<typename T>
T Query::GetResult()
{

}

template<>
int Query::GetResult<int>()
{
	int val;

	glGetQueryObjectiv(handle, GL_QUERY_RESULT, &val);

	return val;
}