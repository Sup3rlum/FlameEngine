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
long long Memory::ToLLong(BYTE* data)
{
	long long t = 0;

	memcpy(&t, data, sizeof(BYTE) * 8);

	return t;
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