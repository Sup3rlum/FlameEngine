#include "D3D12FRICommon.h"

FD3D12RootSignature::FD3D12RootSignature(FD3D12Device* Parent, FArray<FRIRootParameter> RootParameters, FArray<FRIStaticSampler> Samplers) :
    FD3D12DeviceChild(Parent)
{
    auto Device = Parent->GetDevice();
    auto Desc = CreateD3D12RootSignatureDesc(RootParameters, Samplers);

    /* Enumerate Root Parameters */
    for (int ParamIdx = 0; ParamIdx < RootParameters.Length(); ParamIdx++)
    {
        const auto& RootParam = Desc.Desc_1_1.pParameters[ParamIdx];

        EShaderFrequency CurrentVisibleSF = SF_ShaderStages;
        switch (RootParam.ShaderVisibility)
        {
        case D3D12_SHADER_VISIBILITY_ALL:
            CurrentVisibleSF = SF_ShaderStages;
            break;

        case D3D12_SHADER_VISIBILITY_VERTEX:
            CurrentVisibleSF = SF_Vertex;
            break;
        case D3D12_SHADER_VISIBILITY_HULL:
            CurrentVisibleSF = SF_Hull;
            break;
        case D3D12_SHADER_VISIBILITY_DOMAIN:
            CurrentVisibleSF = SF_Domain;
            break;
        case D3D12_SHADER_VISIBILITY_GEOMETRY:
            CurrentVisibleSF = SF_Geometry;
            break;
        case D3D12_SHADER_VISIBILITY_PIXEL:
            CurrentVisibleSF = SF_Pixel;
            break;
        }

        switch (RootParam.ParameterType)
        {
        case D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE:
        {
            const auto& Range = RootParam.DescriptorTable.pDescriptorRanges[0];

            switch (Range.RangeType)
            {
            case D3D12_DESCRIPTOR_RANGE_TYPE_SRV:
                SetSRVTableSlot(CurrentVisibleSF, ParamIdx);
                break;
            case D3D12_DESCRIPTOR_RANGE_TYPE_CBV:
                SetCBVTableSlot(CurrentVisibleSF, ParamIdx);
                break;
            case D3D12_DESCRIPTOR_RANGE_TYPE_UAV:
                SetUAVTableSlot(/*CurrentVisibleSF,*/ ParamIdx);
                break;
            case D3D12_DESCRIPTOR_RANGE_TYPE_SAMPLER:
                SetSamplerTableSlot(CurrentVisibleSF, ParamIdx);
                break;

            default: break;
            }
        }
        break;
        }
    }

    ID3DBlob* errorBuff; // a buffer holding the error data if any
    ID3DBlob* signature;
    HRESULT hr;

    hr = D3D12SerializeVersionedRootSignature(&Desc, &signature, &errorBuff);
    if (FAILED(hr))
    {
        OutputDebugStringA((char*)errorBuff->GetBufferPointer());
    }

    ThrowIfFailed(Device->CreateRootSignature(0, signature->GetBufferPointer(), signature->GetBufferSize(), IID_PPV_ARGS(&RootSignature)));
}