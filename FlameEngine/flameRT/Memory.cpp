#include "Memory.h"

float Memory::ToSingle(BYTE* data)
{
	float t = 0;

	memcpy(&t, data, sizeof(BYTE) * 4);

	return t;
}
int Memory::ToInt(BYTE* data)
{
	int t = 0;

	memcpy(&t, data, sizeof(BYTE) * 4);

	return t;
}

double Memory::ToDouble(BYTE* data)
{
	double t = 0;

	memcpy(&t, data, sizeof(BYTE) * 8);

	return t;
}
FL_INT64 Memory::ToLLong(BYTE* data)
{
	long long t = 0;

	memcpy(&t, data, sizeof(BYTE) * 8);

	return t;
}
_UNS_ FL_INT64 Memory::ToULLong(BYTE* data)
{
	long long t = 0;

	memcpy(&t, data, sizeof(BYTE) * 8);

	return t;
}


STRING Memory::ToString(BYTE* data, int length)
{
	char* str = Create<char>(length + 1);

	memcpy(str, data, length);

	str[length] = '\0';

	return STRING(str);
}

void Memory::GetData(float f, BYTE* data)
{
	memcpy(data, &f, sizeof(BYTE) * 4);
}
void Memory::GetData(int i, BYTE* data)
{
	memcpy(data, &i, sizeof(BYTE) * 4);
}
void Memory::GetData(double d, BYTE* data)
{
	memcpy(data, &d, sizeof(BYTE) * 8);
}
void Memory::GetData(long long ll, BYTE* data)
{
	memcpy(data, &ll, sizeof(BYTE) * 8);
}