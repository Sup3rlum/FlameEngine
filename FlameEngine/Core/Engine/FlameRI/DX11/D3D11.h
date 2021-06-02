#pragma once


#include <d3d11.h>
#include <d3dcompiler.h>

template<typename TD3DTag>
struct TComPtr
{
private:
    typedef TD3DTag* TD3DTagPtr;

public:
    TComPtr(TD3DTagPtr pTagPtr) :
        pD3dResource(pTagPtr)
    {}

    TComPtr() :
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


    operator TD3DTagPtr()
    {
        return pD3dResource;
    }

    ~TComPtr()
    {
        if (pD3dResource != NULL)
        {
            pD3dResource->Release();
            pD3dResource = NULL;
        }
    }
private:
    TD3DTagPtr pD3dResource;
};