#pragma once

enum ERootParameterKeys
{
    PS_SRVs,
    PS_CBVs,
    PS_RootCBVs,
    PS_Samplers,
    VS_SRVs,
    VS_CBVs,
    VS_RootCBVs,
    VS_Samplers,
    GS_SRVs,
    GS_CBVs,
    GS_RootCBVs,
    GS_Samplers,
    HS_SRVs,
    HS_CBVs,
    HS_RootCBVs,
    HS_Samplers,
    DS_SRVs,
    DS_CBVs,
    DS_RootCBVs,
    DS_Samplers,
    ALL_SRVs,
    ALL_CBVs,
    ALL_RootCBVs,
    ALL_Samplers,
    ALL_UAVs,
    RPK_RootParameterKeyCount,
};

FORCEINLINE D3D12_SHADER_VISIBILITY CreateD3D12ShaderVisibility(EFRIShaderVisibility shaderVis)
{
    if (shaderVis == EFRIShaderVisibility::All) return D3D12_SHADER_VISIBILITY_ALL;
    if (shaderVis == EFRIShaderVisibility::Vertex) return D3D12_SHADER_VISIBILITY_VERTEX;
    if (shaderVis == EFRIShaderVisibility::Pixel) return D3D12_SHADER_VISIBILITY_PIXEL;
    if (shaderVis == EFRIShaderVisibility::Hull) return D3D12_SHADER_VISIBILITY_HULL;
    if (shaderVis == EFRIShaderVisibility::Domain) return D3D12_SHADER_VISIBILITY_DOMAIN;
    if (shaderVis == EFRIShaderVisibility::Geometry) return D3D12_SHADER_VISIBILITY_GEOMETRY;

    return D3D12_SHADER_VISIBILITY_ALL;
}

FORCEINLINE D3D12_ROOT_PARAMETER_TYPE CreateD3D12RootParameterType(EFRIRootParameterType paramType)
{
    if (paramType == EFRIRootParameterType::DESCRIPTOR_TABLE) return D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
    else if (paramType == EFRIRootParameterType::CONSTANTS) return D3D12_ROOT_PARAMETER_TYPE_32BIT_CONSTANTS;
    else if (paramType == EFRIRootParameterType::CBV) return D3D12_ROOT_PARAMETER_TYPE_CBV;
    else if (paramType == EFRIRootParameterType::UAV) return D3D12_ROOT_PARAMETER_TYPE_UAV;
    else if (paramType == EFRIRootParameterType::SRV) return D3D12_ROOT_PARAMETER_TYPE_SRV;

    return D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
}

FORCEINLINE D3D12_DESCRIPTOR_RANGE_TYPE CreateD3D12DescriptorRangeType(EFRIRootDescriptorRangeType rangeType)
{
    if (rangeType == EFRIRootDescriptorRangeType::SRV) return D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
    else if (rangeType == EFRIRootDescriptorRangeType::CBV) return D3D12_DESCRIPTOR_RANGE_TYPE_CBV;
    else if (rangeType == EFRIRootDescriptorRangeType::UAV) return D3D12_DESCRIPTOR_RANGE_TYPE_UAV;
}

FORCEINLINE D3D12_STATIC_SAMPLER_DESC CreateD3D12StaticSampler(FRIStaticSampler StaticSampler)
{
    float MaxAniso = 4.0f;

    if (StaticSampler.Filter == EFRITextureFilter::Anisotropic4) MaxAniso = 4.0f;
    if (StaticSampler.Filter == EFRITextureFilter::Anisotropic8) MaxAniso = 8.0f;

    D3D12_STATIC_SAMPLER_DESC SamplerDesc;

    SamplerDesc.Filter = D3D12GetTextureFilter(StaticSampler.Filter);
    SamplerDesc.AddressU = D3D12GetTextureAddressMode(StaticSampler.AddressU);
    SamplerDesc.AddressV = D3D12GetTextureAddressMode(StaticSampler.AddressV);
    SamplerDesc.AddressW = D3D12GetTextureAddressMode(StaticSampler.AddressW);
    SamplerDesc.MaxAnisotropy = MaxAniso;
    SamplerDesc.ComparisonFunc = D3D12_COMPARISON_FUNC_ALWAYS;
    SamplerDesc.BorderColor = D3D12_STATIC_BORDER_COLOR_TRANSPARENT_BLACK;
    SamplerDesc.MinLOD = 0;
    SamplerDesc.MaxLOD = D3D12_FLOAT32_MAX;
    SamplerDesc.MipLODBias = 0;
    SamplerDesc.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
    SamplerDesc.ShaderRegister = StaticSampler.ShaderRegister;
    SamplerDesc.RegisterSpace = StaticSampler.RegisterSpace;

    return SamplerDesc;
}

FORCEINLINE D3D12_INPUT_LAYOUT_DESC CreateD3D12InputLayoutDesc(FRIInputLayout InputLayout)
{

    auto ElementDescs = new D3D12_INPUT_ELEMENT_DESC[InputLayout.NumElements];

    uint32 AlignedByteOffset = 0;
    for (int Elem = 0; Elem < InputLayout.NumElements; Elem++)
    {
        const auto& FRIElem = InputLayout.DeclarationElements[Elem];
        ElementDescs[Elem] = {
            .SemanticName = FRIElem.Semantic.SemanticName,
            .SemanticIndex = FRIElem.Semantic.SemanticIndex,
            .Format = FRIAttributeFormatToDXGI(FRIElem.Type),
            .InputSlot = 0,
            .AlignedByteOffset = AlignedByteOffset,
            .InputSlotClass = FRIAttributeUsageFormatToClassification(FRIElem.Usage),
            .InstanceDataStepRate = 0
        };

        AlignedByteOffset += FRIAttributeFormatByteSize(FRIElem.Type);
    }

    return D3D12_INPUT_LAYOUT_DESC
    {
        .pInputElementDescs = ElementDescs,
        .NumElements = InputLayout.NumElements
    };
}

FORCEINLINE D3D12_VERSIONED_ROOT_SIGNATURE_DESC CreateD3D12RootSignatureDesc(FArray<FRIRootParameter> RootParameters, FArray<FRIStaticSampler> StaticSamplers)
{
    auto RootParams = new D3D12_ROOT_PARAMETER1[RootParameters.Length()];
    for (int Param = 0; Param < RootParameters.Length(); Param++)
    {
        const auto& FRIParam = RootParameters[Param];

        switch (FRIParam.ParamType)
        {
            case EFRIRootParameterType::DESCRIPTOR_TABLE:
            {
                auto Ranges = new D3D12_DESCRIPTOR_RANGE1[FRIParam.DescriptorTable.NumRanges];

                for (int Range = 0; Range < FRIParam.DescriptorTable.NumRanges; Range++)
                {
                    const auto& FRIRange = FRIParam.DescriptorTable.Ranges[Range];
                    Ranges[Range] = {
                        .RangeType = CreateD3D12DescriptorRangeType(FRIRange.RangeType),
                        .NumDescriptors = FRIRange.NumDescriptors,
                        .BaseShaderRegister = FRIRange.BaseShaderRegister,
                        .RegisterSpace = FRIRange.RegisterSpace,
                        .Flags = D3D12_DESCRIPTOR_RANGE_FLAG_DESCRIPTORS_VOLATILE,
                        .OffsetInDescriptorsFromTableStart = FRIRange.OffsetInDescriptorsFromTableStart
                    };
                }

                RootParams[Param] = {
                    .ParameterType = CreateD3D12RootParameterType(FRIParam.ParamType),
                    .DescriptorTable = {
                        .NumDescriptorRanges = FRIParam.DescriptorTable.NumRanges,
                        .pDescriptorRanges = Ranges
                        },
                    .ShaderVisibility = CreateD3D12ShaderVisibility(FRIParam.ShaderVisibility),
                };
                break;
            }
            case EFRIRootParameterType::CONSTANTS:
            {
                RootParams[Param] = {
                    .ParameterType = CreateD3D12RootParameterType(FRIParam.ParamType),
                    .Constants = {
                        .ShaderRegister = 0,
                        .RegisterSpace = 0,
                        .Num32BitValues = 0
                    },
                    .ShaderVisibility = CreateD3D12ShaderVisibility(FRIParam.ShaderVisibility)
                };
                break;
            }
            case EFRIRootParameterType::CBV:
            case EFRIRootParameterType::SRV:
            case EFRIRootParameterType::UAV:
            {
                RootParams[Param] = {
                    .ParameterType = CreateD3D12RootParameterType(FRIParam.ParamType),
                    .Descriptor = {
                        .ShaderRegister = FRIParam.Descriptor.ShaderRegister,
                        .RegisterSpace = FRIParam.Descriptor.RegisterSpace,
                        .Flags = (D3D12_ROOT_DESCRIPTOR_FLAGS)0,
                        
                    },
                    .ShaderVisibility = CreateD3D12ShaderVisibility(FRIParam.ShaderVisibility)
                };
                break;
            }
        };
    }

    auto Samplers = new D3D12_STATIC_SAMPLER_DESC[StaticSamplers.Length()];
    for (int Sampler = 0; Sampler < StaticSamplers.Length(); Sampler++)
    {
        Samplers[Sampler] = CreateD3D12StaticSampler(StaticSamplers[Sampler]);
    }

    D3D12_ROOT_SIGNATURE_DESC1 RootSignatureDesc;
    RootSignatureDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT | // we can deny shader stages here for better performance
        D3D12_ROOT_SIGNATURE_FLAG_DENY_HULL_SHADER_ROOT_ACCESS |
        D3D12_ROOT_SIGNATURE_FLAG_DENY_DOMAIN_SHADER_ROOT_ACCESS |
        D3D12_ROOT_SIGNATURE_FLAG_DENY_GEOMETRY_SHADER_ROOT_ACCESS;
    RootSignatureDesc.NumParameters = RootParameters.Length();
    RootSignatureDesc.pParameters = RootParams;
    RootSignatureDesc.NumStaticSamplers = StaticSamplers.Length();
    RootSignatureDesc.pStaticSamplers = Samplers;

    D3D12_VERSIONED_ROOT_SIGNATURE_DESC VersionedDesc = {};
    VersionedDesc.Version = D3D_ROOT_SIGNATURE_VERSION_1_1;
    VersionedDesc.Desc_1_1 = RootSignatureDesc;

    return VersionedDesc;

}

FORCEINLINE D3D12_RASTERIZER_DESC CreateD3D12RasterizerDesc(FRIRasterizerState RasterizerState)
{
    D3D12_RASTERIZER_DESC Desc{};

    auto CullMode = D3D12_CULL_MODE_BACK;
    switch (RasterizerState.CullMode)
    {
    case EFRICullMode::Front: CullMode = D3D12_CULL_MODE_FRONT; break;
    case EFRICullMode::Back: CullMode = D3D12_CULL_MODE_BACK; break;
    case EFRICullMode::None: CullMode = D3D12_CULL_MODE_NONE; break;
    default:
        break;
    }

    auto FillMode = D3D12_FILL_MODE_SOLID;
    switch (RasterizerState.FillMode)
    {
    case EFRIFillMode::Solid: FillMode = D3D12_FILL_MODE_SOLID; break;
    case EFRIFillMode::Wireframe: FillMode = D3D12_FILL_MODE_WIREFRAME; break;
    default:
        break;
    }

    Desc.FillMode = FillMode;
    Desc.CullMode = CullMode;
    Desc.FrontCounterClockwise = TRUE;
    Desc.DepthBias = D3D12_DEFAULT_DEPTH_BIAS;
    Desc.DepthBiasClamp = D3D12_DEFAULT_DEPTH_BIAS_CLAMP;
    Desc.SlopeScaledDepthBias = D3D12_DEFAULT_SLOPE_SCALED_DEPTH_BIAS;
    Desc.DepthClipEnable = TRUE;
    Desc.MultisampleEnable = FALSE;
    Desc.AntialiasedLineEnable = FALSE;
    Desc.ForcedSampleCount = 0;
    Desc.ConservativeRaster = D3D12_CONSERVATIVE_RASTERIZATION_MODE_OFF;

    return Desc;
}

FORCEINLINE D3D12_DEPTH_STENCIL_DESC CreateD3D12DepthStencilDesc(FRIDepthStencilState DepthStencilState)
{
    D3D12_DEPTH_STENCIL_DESC Desc{};
    Desc.DepthEnable = DepthStencilState.EnableDepth == EFRIBool::True ? TRUE : FALSE;
    Desc.DepthFunc = D3D12_COMPARISON_FUNC_LESS;
    Desc.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;

    return Desc;
}

FORCEINLINE D3D12_BLEND_DESC CreateD3D12BlendDesc(FRIBlendState BlendState)
{

    D3D12_RENDER_TARGET_BLEND_DESC BlendDesc{};

    BlendDesc.BlendEnable = TRUE;
    BlendDesc.LogicOpEnable = FALSE;
    BlendDesc.SrcBlend = D3D12GetBlend(BlendState.SrcBlend);
    BlendDesc.DestBlend = D3D12GetBlend(BlendState.DstBlend);
    BlendDesc.BlendOp = D3D12_BLEND_OP_ADD;
    BlendDesc.SrcBlendAlpha = D3D12_BLEND_ONE;
    BlendDesc.DestBlendAlpha = D3D12_BLEND_ZERO;
    BlendDesc.BlendOpAlpha = D3D12_BLEND_OP_ADD;
    BlendDesc.LogicOp = D3D12_LOGIC_OP_NOOP;
    BlendDesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
    D3D12_BLEND_DESC Desc{};
    for (int idx = 0; idx < 8; idx++)
    {
        Desc.RenderTarget[idx] = BlendDesc;
    }
    return Desc;
}

class FD3D12RootSignature: public FD3D12DeviceChild
{
public:
    ID3D12RootSignature* RootSignature;
    uint32 BindSlotMap[RPK_RootParameterKeyCount] = { 0 };

    FD3D12RootSignature(FD3D12Device* Parent, FArray<FRIRootParameter> RootParameters, FArray<FRIStaticSampler> Samplers);

    FORCEINLINE void SetSamplerTableSlot(EShaderFrequency ShaderFrequency, uint32 RootParameterIndex)
    {
        uint32* pBindSlot = nullptr;
        switch (ShaderFrequency)
        {
        case SF_Vertex: pBindSlot = &BindSlotMap[VS_Samplers]; break;
        case SF_Pixel: pBindSlot = &BindSlotMap[PS_Samplers]; break;
        case SF_Geometry: pBindSlot = &BindSlotMap[GS_Samplers]; break;
        case SF_Hull: pBindSlot = &BindSlotMap[HS_Samplers]; break;
        case SF_Domain: pBindSlot = &BindSlotMap[DS_Samplers]; break;

        case SF_Compute:
        case SF_ShaderStages: pBindSlot = &BindSlotMap[ALL_Samplers]; break;

        default:
            return;
        }

        *pBindSlot = RootParameterIndex;
    }

    FORCEINLINE void SetSRVTableSlot(EShaderFrequency ShaderFrequency, uint32 RootParameterIndex)
    {
        uint32* pBindSlot = nullptr;
        switch (ShaderFrequency)
        {
        case SF_Vertex: pBindSlot = &BindSlotMap[VS_SRVs]; break;
        case SF_Pixel: pBindSlot = &BindSlotMap[PS_SRVs]; break;
        case SF_Geometry: pBindSlot = &BindSlotMap[GS_SRVs]; break;
        case SF_Hull: pBindSlot = &BindSlotMap[HS_SRVs]; break;
        case SF_Domain: pBindSlot = &BindSlotMap[DS_SRVs]; break;

        case SF_Compute:
        case SF_ShaderStages: pBindSlot = &BindSlotMap[ALL_SRVs]; break;

        default:
            return;
        }

        *pBindSlot = RootParameterIndex;
    }

    FORCEINLINE void SetCBVTableSlot(EShaderFrequency ShaderFrequency, uint32 RootParameterIndex)
    {
        uint32* pBindSlot = nullptr;
        switch (ShaderFrequency)
        {
        case SF_Vertex: pBindSlot = &BindSlotMap[VS_CBVs]; break;
        case SF_Pixel: pBindSlot = &BindSlotMap[PS_CBVs]; break;
        case SF_Geometry: pBindSlot = &BindSlotMap[GS_CBVs]; break;
        case SF_Hull: pBindSlot = &BindSlotMap[HS_CBVs]; break;
        case SF_Domain: pBindSlot = &BindSlotMap[DS_CBVs]; break;

        case SF_Compute:
        case SF_ShaderStages: pBindSlot = &BindSlotMap[ALL_CBVs]; break;

        default:
            return;
        }

        *pBindSlot = RootParameterIndex;
    }

    FORCEINLINE void SetUAVTableSlot(uint32 RootParameterIndex)
    {
        uint32* pBindSlot = &BindSlotMap[ALL_UAVs];
        *pBindSlot = RootParameterIndex;
    }


    inline uint32 SamplerTableSlot(EShaderFrequency ShaderStage) const
    {
        switch (ShaderStage)
        {
        case SF_Vertex: return BindSlotMap[VS_Samplers];
        case SF_Pixel: return BindSlotMap[PS_Samplers];
        case SF_Geometry: return BindSlotMap[GS_Samplers];
        case SF_Hull: return BindSlotMap[HS_Samplers];
        case SF_Domain: return BindSlotMap[DS_Samplers];
        case SF_Compute: return BindSlotMap[ALL_Samplers];

        default:
            return UINT_MAX;
        }
    }

    inline uint32 SRVTableSlot(EShaderFrequency ShaderStage) const
    {
        switch (ShaderStage)
        {
        case SF_Vertex: return BindSlotMap[VS_SRVs];
        case SF_Pixel: return BindSlotMap[PS_SRVs];
        case SF_Geometry: return BindSlotMap[GS_SRVs];
        case SF_Hull: return BindSlotMap[HS_SRVs];
        case SF_Domain: return BindSlotMap[DS_SRVs];
        case SF_Compute: return BindSlotMap[ALL_SRVs];

        default:
            return UINT_MAX;
        }
    }

    inline uint32 CBVTableSlot(EShaderFrequency ShaderStage) const
    {
        switch (ShaderStage)
        {
        case SF_Vertex: return BindSlotMap[VS_CBVs];
        case SF_Pixel: return BindSlotMap[PS_CBVs];
        case SF_Geometry: return BindSlotMap[GS_CBVs];
        case SF_Hull: return BindSlotMap[HS_CBVs];
        case SF_Domain: return BindSlotMap[DS_CBVs];
        case SF_Compute: return BindSlotMap[ALL_CBVs];

        default:
            return UINT_MAX;
        }
    }

    inline uint32 UAVTableSlot(EShaderFrequency ShaderStage) const
    {
        return BindSlotMap[ALL_UAVs];
    }

};

class FD3D12PipelineStateObject : public FRIPipelineStateObject, public FD3D12DeviceChild
{
public:
    ComPtr<ID3D12PipelineState> PipelineStateObject;
    FD3D12RootSignature* RootSignature;

    FD3D12PipelineStateObject
    (
        FD3D12Device* InParent,
        FD3D12RootSignature* RootSignature) :
        FD3D12DeviceChild(InParent),
        RootSignature(RootSignature)
    {
    }

};

class FD3D12GraphicsPipelineStateObject : public FD3D12PipelineStateObject
{
public:
    FD3D12GraphicsPipelineStateObject
    (
        FD3D12Device* InParent,
        FRIInputLayout InputLayoutDesc,
        FD3D12RootSignature* RootSignature,
        D3D12_SHADER_BYTECODE VSByteCode,
        D3D12_SHADER_BYTECODE PSByteCode,
        D3D12_SHADER_BYTECODE GSByteCode,
        D3D12_SHADER_BYTECODE HSByteCode,
        D3D12_SHADER_BYTECODE DSByteCode,
        FArray<EFRITextureFormat> RTVFormats,
        FRIRasterizerState RasterizerState,
        FRIDepthStencilState DepthStencilState,
        FRIBlendState BlendState
        ) : 
        FD3D12PipelineStateObject(InParent, RootSignature)
	{

        auto Device = InParent->GetDevice();

        D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc = {}; // a structure to define a pso
        psoDesc.InputLayout = CreateD3D12InputLayoutDesc(InputLayoutDesc); // the structure describing our input layout
        psoDesc.pRootSignature = RootSignature->RootSignature; // the root signature that describes the input data this pso needs
        psoDesc.VS = VSByteCode; // structure describing where to find the vertex shader bytecode and how large it is
        psoDesc.PS = PSByteCode; // same as VS but for pixel shader
        psoDesc.DS = DSByteCode;
        psoDesc.HS = HSByteCode;
        psoDesc.GS = GSByteCode;

        psoDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE; // type of topology we are drawing
        psoDesc.SampleDesc.Count = 1;
        psoDesc.SampleDesc.Quality = 0;
        psoDesc.SampleMask = 0xffffffff; // sample mask has to do with multi-sampling. 0xffffffff means point sampling is done

        psoDesc.RasterizerState     = CreateD3D12RasterizerDesc(RasterizerState);
        psoDesc.DepthStencilState   = CreateD3D12DepthStencilDesc(DepthStencilState);
        psoDesc.BlendState          = CreateD3D12BlendDesc(BlendState);
        psoDesc.DSVFormat           = DXGI_FORMAT_D32_FLOAT;

        for (int FormatIdx = 0; FormatIdx < RTVFormats.Length(); FormatIdx++)
        {
            psoDesc.RTVFormats[FormatIdx] = EDX12FormatProxyEnum(RTVFormats[FormatIdx]); // format of the render target
        }
        psoDesc.NumRenderTargets = RTVFormats.Length(); 


        ThrowIfFailed(Device->CreateGraphicsPipelineState(&psoDesc, IID_PPV_ARGS(PipelineStateObject.GetAddressOf())));
	}
};

class FD3D12ComputePipelineStateObject : public FD3D12PipelineStateObject
{
public:
    FD3D12ComputePipelineStateObject
    (
        FD3D12Device* InParent,
        FD3D12RootSignature* RootSignature,
        D3D12_SHADER_BYTECODE CSByteCode
    ) :
        FD3D12PipelineStateObject(InParent, RootSignature)
    {
        auto Device = InParent->GetDevice();

        D3D12_COMPUTE_PIPELINE_STATE_DESC psoDesc = {};
        psoDesc.pRootSignature = RootSignature->RootSignature; 
        psoDesc.CS = CSByteCode; 
        psoDesc.Flags = D3D12_PIPELINE_STATE_FLAG_NONE;

        ThrowIfFailed(Device->CreateComputePipelineState(&psoDesc, IID_PPV_ARGS(PipelineStateObject.GetAddressOf())));
    }
};