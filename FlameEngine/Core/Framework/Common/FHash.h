#pragma once


#include "FString.h"



EXPORT(struct, FHashFuncImpl)
{
public:
    static void MurmurHash3_x64_128(const void* key, const int len, const uint32_t seed, void* out);
    static uint32_t MurmurHash3_x86_32(const void* key, int len, uint32_t seed);
};


template<typename TType>
struct FHash
{
    size_t operator ()(const TType& typePtr) const
    {
        return FHashFuncImpl::MurmurHash3_x86_32(&typePtr, sizeof(TType), 100);
    }
};

template<typename TType>
struct FHash<TType*>
{
    size_t operator ()(TType* typePtr) const
    {
        return FHashFuncImpl::MurmurHash3_x86_32(&static_cast<size_t>(typePtr), sizeof(size_t), 100);
    }
};


template<>
struct FHash<FString>
{
    size_t operator ()(const FString& str) const
    {
        return FHashFuncImpl::MurmurHash3_x86_32(str.Begin(), str.ByteSize(), 100);
    }
};

template<>
struct FHash<FAnsiString>
{
    size_t operator ()(const FAnsiString & str) const
    {
        return FHashFuncImpl::MurmurHash3_x86_32(str.Begin(), str.ByteSize(), 100);
    }
};