#pragma once


#include <wrl/client.h>
#include <cassert>

#include <exception>


template<typename D3DType>
using ComPtr = Microsoft::WRL::ComPtr<D3DType>;

/*
template<typename TD3DTag>
struct ComPtr
{
private:
    typedef TD3DTag* TD3DTagPtr;

public:
    ComPtr(TD3DTagPtr pTagPtr) :
        pD3dResource(pTagPtr)
    {}

    ComPtr() :
        pD3dResource(0)
    {}

    const TD3DTagPtr operator->() const
    {
        return pD3dResource;
    }

    TD3DTagPtr* operator&()
    {
        return &pD3dResource;
    }

    TD3DTagPtr Get()
    {
        return pD3dResource;
    }

    TD3DTagPtr* GetAddressOf()
    {
        return &pD3dResource;
    }

    ~ComPtr()
    {
        if (pD3dResource != NULL)
        {
            pD3dResource->Release();
            pD3dResource = NULL;
        }
    }
private:
    TD3DTagPtr pD3dResource;
};*/


inline void _ThrowIfFailedHRESULT(HRESULT hr)
{
	if (FAILED(hr)) throw std::exception();
}

#define D3D_HRESULT(x) _ThrowIfFailedHRESULT(x)
