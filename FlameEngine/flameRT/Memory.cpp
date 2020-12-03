#include "Memory.h"

float Memory::ToSingle(BYTE* data)
{
	return ToType<float>(data);
}
int Memory::ToInt(BYTE* data)
{
	return ToType<int>(data);
}

double Memory::ToDouble(BYTE* data)
{
	return ToType<double>(data);
}
FL_INT64 Memory::ToLLong(BYTE* data)
{
	return ToType<long long>(data);
}
_UNS_ FL_INT64 Memory::ToULLong(BYTE* data)
{
	return ToType<unsigned long long>(data);
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



template<typename T>
void Memory::GetByteData(T t, _Out_ BYTE* data)
{
	memcpy(data, &t, sizeof(T));
}
