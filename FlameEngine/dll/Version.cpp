#include "Version.h"

std::string DVERSION::ToString(char delim)
{
	std::stringstream ss;

	ss << major << delim << minor << delim << revision <<  delim << build;

	return ss.str();
}
std::string DVERSION::ToString()
{
	return ToString('.');
}
int DVERSION::ToInt()
{
	int v;

	std::istringstream iss(ToString(NULL));
	iss >> v;

	return v;
}

DVERSION::DVERSION(int a, int b, int c, int d)
{
	major = a;
	minor = b;
	revision = c;
	build = d;
}
DVERSION::DVERSION() : DVERSION(0, 0, 0, 0)
{

}