#pragma once

#include <windows.h>
#include <d3d12.h>
#include "d3dx12.h"
#include <dxgi1_6.h>
#include <d3dcompiler.h>
#include <wrl.h>
#include <string>
#include <stdexcept>
//#include "../D3D11/D3DUtil.h"


template<typename D3DType>
using ComPtr = Microsoft::WRL::ComPtr<D3DType>;

inline std::string HrToString(HRESULT hr)
{
    char s_str[64] = {};
    sprintf_s(s_str, "HRESULT of 0x%08X", static_cast<UINT>(hr));
    return std::string(s_str);
}

class HrException : public std::runtime_error
{
public:
    HrException(HRESULT hr) : std::runtime_error(HrToString(hr)), m_hr(hr) {}
    HRESULT Error() const { return m_hr; }
private:
    const HRESULT m_hr;
};

#define SAFE_RELEASE(p) if (p) (p)->Release()

inline void ThrowIfFailed(HRESULT hr)
{
    if (FAILED(hr))
    {
        throw HrException(hr);
    }
}

// Assign a name to the object to aid with debugging.
#if defined(_DEBUG) || defined(DBG)
inline void SetName(ID3D12Object* pObject, LPCWSTR name)
{
    pObject->SetName(name);
}
inline void SetNameIndexed(ID3D12Object* pObject, LPCWSTR name, UINT index)
{
    WCHAR fullName[50];
    if (swprintf_s(fullName, L"%s[%u]", name, index) > 0)
    {
        pObject->SetName(fullName);
    }
}
#else
inline void SetName(ID3D12Object*, LPCWSTR)
{
}
inline void SetNameIndexed(ID3D12Object*, LPCWSTR, UINT)
{
}
#endif

// Naming helper for ComPtr<T>.
// Assigns the name of the variable as the name of the object.
// The indexed variant will include the index in the name of the object.
#define NAME_D3D12_OBJECT(x) SetName((x).Get(), L#x)
#define NAME_D3D12_OBJECT_INDEXED(x, n) SetNameIndexed((x)[n].Get(), L#x, n)


class DX12
{
public:
    template<typename TD3D12Type, typename TFRIType>
    static FORCEINLINE TD3D12Type* Cast(TFRIType* FRIObj)
    {
        return static_cast<TD3D12Type*>(FRIObj);
    }

    template<typename TD3D12Type, typename TFRIType>
    static FORCEINLINE TD3D12Type* DynCast(TFRIType* FRIObj)
    {
        return dynamic_cast<TD3D12Type*>(FRIObj);
    }

    template<typename TD3D12Type, typename TFRIType>
    static FORCEINLINE const TD3D12Type* Cast(const TFRIType* FRIObj)
    {
        return static_cast<TD3D12Type*>(FRIObj);
    }
};