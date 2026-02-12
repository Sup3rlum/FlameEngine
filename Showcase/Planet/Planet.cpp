#include "Planet.h"

const float M_PI = 3.141592;
const int MAX_CRATERS = 1024;


float craterDistribution(float xmin = 0.001f, float xmax = 1.0f, float alpha = 3.5f) {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    static std::uniform_real_distribution<float> dist(0.0f, 1.0f);

    float u = dist(gen); // uniform random [0,1)

    float amin = std::pow(xmin, 1.0f - alpha);
    float amax = std::pow(xmax, 1.0f - alpha);

    float val = std::pow(amin + (amax - amin) * u, 1.0f / (1.0f - alpha));
    return val;
}


FRIPipelineStateObject* Planet::CreateGeometryPipelineState(const ShaderLibraryModule& Shaders)
{
    auto Allocator = FriContext->GetFRIDynamic();
    FArray<EFRITextureFormat> RTVFormats =
    {
        EFRITextureFormat::RGBA8UNORM,
        EFRITextureFormat::RGBA8UNORM,
        EFRITextureFormat::RGBA8UNORM,
        EFRITextureFormat::RGBA8UNORM
    };
    FRIInputLayout StaticLitLayout = {
        .NumElements = 5,
        .DeclarationElements = new FRIInputElementDesc[5]
        {
            FRIInputElementDesc("POSITION", EFRIAttributeType::Float3, 0, EFRIAttribUsage::PerVertex),
            FRIInputElementDesc("NORMAL", EFRIAttributeType::Float3, 1, EFRIAttribUsage::PerVertex),
            FRIInputElementDesc("TANGENT", EFRIAttributeType::Float3, 2, EFRIAttribUsage::PerVertex),
            FRIInputElementDesc("BITANGENT", EFRIAttributeType::Float3, 3, EFRIAttribUsage::PerVertex),
            FRIInputElementDesc("TEXCOORD", EFRIAttributeType::Float2, 4, EFRIAttribUsage::PerVertex)
        }
    };

    // Texture 0

    FRIDescriptorRange Texture0Range;
    Texture0Range.NumDescriptors = 4;
    Texture0Range.BaseShaderRegister = 0;
    Texture0Range.RegisterSpace = 0;
    Texture0Range.OffsetInDescriptorsFromTableStart = 0xffffffff;
    Texture0Range.RangeType = EFRIRootDescriptorRangeType::SRV;

    FRIRootDescriptorTable Textures;
    Textures.NumRanges = 1;
    Textures.Ranges = &Texture0Range;

    // CBV

    FRIDescriptorRange ConstantBuffer1Range;
    ConstantBuffer1Range.NumDescriptors = 2;
    ConstantBuffer1Range.BaseShaderRegister = 0;
    ConstantBuffer1Range.RegisterSpace = 0;
    ConstantBuffer1Range.OffsetInDescriptorsFromTableStart = 0xffffffff;
    ConstantBuffer1Range.RangeType = EFRIRootDescriptorRangeType::CBV;

    FRIRootDescriptorTable ConstantBufferTable;
    ConstantBufferTable.NumRanges = 1;
    ConstantBufferTable.Ranges = &ConstantBuffer1Range;

    FArray<FRIRootParameter> RootParams = {
            FRIRootParameter(EFRIRootParameterType::DESCRIPTOR_TABLE, EFRIShaderVisibility::Vertex, ConstantBufferTable),
            FRIRootParameter(EFRIRootParameterType::DESCRIPTOR_TABLE, EFRIShaderVisibility::Pixel, ConstantBufferTable),
            FRIRootParameter(EFRIRootParameterType::DESCRIPTOR_TABLE, EFRIShaderVisibility::Pixel, Textures),
    };
    FArray<FRIStaticSampler> StaticSamplers = {
            FRIStaticSampler(EFRITextureFilter::Bilinear, EFRITextureAddress::Repeat, EFRITextureAddress::Repeat, EFRITextureAddress::Repeat, 0, 0),
            FRIStaticSampler(EFRITextureFilter::Bilinear, EFRITextureAddress::Repeat, EFRITextureAddress::Repeat, EFRITextureAddress::Repeat, 1, 0),
            FRIStaticSampler(EFRITextureFilter::Bilinear, EFRITextureAddress::Repeat, EFRITextureAddress::Repeat, EFRITextureAddress::Repeat, 2, 0),
            FRIStaticSampler(EFRITextureFilter::Bilinear, EFRITextureAddress::Repeat, EFRITextureAddress::Repeat, EFRITextureAddress::Repeat, 3, 0),
    };


    FRIRasterizerState Rasterizer{ EFRICullMode::Back , EFRIFillMode::Solid };
    FRIDepthStencilState Depth{ EFRIBool::True };
    FRIBlendState Blend{ EFRIBlend::One, EFRIBlend::Zero };

    return Allocator->CreatePipelineStateObject(
        Shaders,
        StaticLitLayout,
        RootParams,
        StaticSamplers,
        RTVFormats,
        Rasterizer,
        Depth,
        Blend
    );

}


FRIPipelineStateObject* Planet::CreateComputePipeline(const ShaderLibraryModule& Shader, uint32 UAVCount, uint32 CBVCount)
{
    auto Allocator = FriContext->GetFRIDynamic();
    FArray<FRIRootParameter> RootParams;

    // UAV

    FRIDescriptorRange UAV1Range;
    UAV1Range.NumDescriptors = UAVCount;
    UAV1Range.BaseShaderRegister = 0;
    UAV1Range.RegisterSpace = 0;
    UAV1Range.OffsetInDescriptorsFromTableStart = 0xffffffff;
    UAV1Range.RangeType = EFRIRootDescriptorRangeType::UAV;

    FRIRootDescriptorTable UAVTable;
    UAVTable.NumRanges = 1;
    UAVTable.Ranges = &UAV1Range;

    RootParams.Add(FRIRootParameter(EFRIRootParameterType::DESCRIPTOR_TABLE, EFRIShaderVisibility::All, UAVTable));

    // CBV

    if (CBVCount > 0)
    {

        FRIDescriptorRange CBV1Range;
        CBV1Range.NumDescriptors = CBVCount;
        CBV1Range.BaseShaderRegister = 0;
        CBV1Range.RegisterSpace = 0;
        CBV1Range.OffsetInDescriptorsFromTableStart = 0xffffffff;
        CBV1Range.RangeType = EFRIRootDescriptorRangeType::CBV;

        FRIRootDescriptorTable CBVTable;
        CBVTable.NumRanges = 1;
        CBVTable.Ranges = &CBV1Range;

        RootParams.Add(FRIRootParameter(EFRIRootParameterType::DESCRIPTOR_TABLE, EFRIShaderVisibility::All, CBVTable));

    }
    return Allocator->CreateComputePipelineStateObject(RootParams, Shader);
}

Planet::Planet(FRIContext* FriContext) :
	FriContext(FriContext)
{
	auto Allocator = FriContext->GetFRIDynamic();
	FAssetManager Content;
	Content.RenderContext = FriContext;
	Content.Connect("./Assets/");

    material = Content.Load<Material>("Materials/default2.flmt");
    mesh = Content.Load<Mesh>("Models/icosphere.fl3d");
	auto planetaryShaders = Content.Load<ShaderLibrary>("Shaders/planetary_dx.fslib");

    GenerationCompute   = CreateComputePipeline(planetaryShaders.Modules["PlanetGenerator"], 3, 4);
    RenormalizeCompute  = CreateComputePipeline(planetaryShaders.Modules["PlanetRecomputeNormals"], 3, 0);
    RenormalizeCompute2 = CreateComputePipeline(planetaryShaders.Modules["PlanetRecomputeNormals2"], 3, 0);
    PlanetRender        = CreateGeometryPipelineState(planetaryShaders.Modules["PlanetGBuffer"]);

    IntermediateNormalBuffer    = Allocator->CreateComputeBuffer(mesh.VertexBuffer->VertexCount, sizeof(IVector3) * 3, EFRIAccess::ReadWrite, EFRIUsage::Default);
    CraterBuffer                = Allocator->CreateComputeBuffer(MAX_CRATERS, sizeof(Crater), EFRIAccess::None, EFRIUsage::Default);

    CraterParams                = Allocator->CreateConstantBuffer(sizeof(int), EFRIAccess::Read, EFRIUsage::Default);
    OceanNoiseParams            = Allocator->CreateConstantBuffer(sizeof(NoiseParams), EFRIAccess::Read, EFRIUsage::Default);
    ContinentNoiseParams        = Allocator->CreateConstantBuffer(sizeof(NoiseParams), EFRIAccess::Read, EFRIUsage::Default);
    MountainMaskNoiseParams     = Allocator->CreateConstantBuffer(sizeof(NoiseParams), EFRIAccess::Read, EFRIUsage::Default);

    MeshVertexUAV           = Allocator->CreateUnorderedAccessView(mesh.VertexBuffer);
    MeshIndexUAV            = Allocator->CreateUnorderedAccessView(mesh.IndexBuffer);
    CraterBufferUAV         = Allocator->CreateUnorderedAccessView(CraterBuffer);
    IntermediateNormalUAV   = Allocator->CreateUnorderedAccessView(IntermediateNormalBuffer);

    Desc.Craters.Clear();
    for (int32 i = 0; i < MAX_CRATERS; i++)
    {

        Desc.Craters.Add(Crater{ FVector3(), 0, 0, 0, 0 });
    }

    FRICommandList cmdList(FriContext->GetCommandContext(0));

    cmdList.Open();
    Generate();
    cmdList.CloseAndExecute();
}

void Planet::SetOceanNoiseParams(
    float Frequency,
    float Octaves,
    float ScaleWidth,
    float ScaleHeight,
    float Persistence,
    float Lacunarity)
{
    Desc.OceanNoise.Frequency = Frequency;
    Desc.OceanNoise.Octaves = Octaves;
    Desc.OceanNoise.ScaleWidth = ScaleWidth;
    Desc.OceanNoise.ScaleHeight = ScaleHeight;
    Desc.OceanNoise.Persistence = Persistence;
    Desc.OceanNoise.Lacunarity = Lacunarity;

    GenerateMesh(Desc);
}

void Planet::SetContinentNoiseParams(
    float Frequency,
    float Octaves,
    float ScaleWidth,
    float ScaleHeight,
    float Persistence,
    float Lacunarity)
{
    Desc.ContinentNoise.Frequency = Frequency;
    Desc.ContinentNoise.Octaves = Octaves;
    Desc.ContinentNoise.ScaleWidth = ScaleWidth;
    Desc.ContinentNoise.ScaleHeight = ScaleHeight;
    Desc.ContinentNoise.Persistence = Persistence;
    Desc.ContinentNoise.Lacunarity = Lacunarity;

    GenerateMesh(Desc);
}

void Planet::SetMountainMaskNoiseParams(
    float Frequency,
    float Octaves,
    float ScaleWidth,
    float ScaleHeight,
    float Persistence,
    float Lacunarity)
{
    Desc.MountainNoiseMask.Frequency = Frequency;
    Desc.MountainNoiseMask.Octaves = Octaves;
    Desc.MountainNoiseMask.ScaleWidth = ScaleWidth;
    Desc.MountainNoiseMask.ScaleHeight = ScaleHeight;
    Desc.MountainNoiseMask.Persistence = Persistence;
    Desc.MountainNoiseMask.Lacunarity = Lacunarity;

    GenerateMesh(Desc);
}

void Planet::SetCraterParams(int numCraters, float MinRadius, float MaxRadius, float Bias)
{
    FRandom r;

    Desc.Craters.Clear();
    for (int32 i = 0; i < numCraters; i++)
    {
        FVector3 pos(
            r.RandFloat(0, 1) * 2 - 1,
            r.RandFloat(0, 1) * 2 - 1,
            r.RandFloat(0, 1) * 2 - 1
        );

        float radius = craterDistribution(MinRadius, MaxRadius, Bias);
        float depth = radius / 5.0f;
        float rimWidth = radius * 1.5f;
        float rimHeight = depth / 2.0f;

        Desc.Craters.Add(Crater{ FVector3::Normalize(pos), radius, depth, rimWidth, rimHeight });
    }

    GenerateMesh(Desc);
}

void Planet::Generate()
{
    SetContinentNoiseParams(1, 4, 1, 0.2, 0.5, 2.5);

}
void Planet::GenerateMesh(const PlanetDesc& Desc)
{
    FRICommandList cmdList(FriContext->GetCommandContext(0));

    //cmdList.Open();
    cmdList.BeginCompute();

    // Upload crater data
    if (Desc.Craters.Length())
        cmdList.ResourceSubdata(CraterBuffer,               FRIUpdateDescriptor(Desc.Craters.Begin(), 0, Desc.Craters.ByteSize()));

    cmdList.ResourceSubdata(CraterParams,               FRIUpdateDescriptor(new int(Desc.Craters.Length()), 0, sizeof(int)));
    cmdList.ResourceSubdata(OceanNoiseParams,           FRIUpdateDescriptor(&Desc.OceanNoise, 0, sizeof(NoiseParams)));
    cmdList.ResourceSubdata(ContinentNoiseParams,       FRIUpdateDescriptor(&Desc.ContinentNoise, 0, sizeof(NoiseParams)));
    cmdList.ResourceSubdata(MountainMaskNoiseParams,    FRIUpdateDescriptor(&Desc.MountainNoiseMask, 0, sizeof(NoiseParams)));

    // Deform sphere
    cmdList.SetPipelineState(GenerationCompute);
    cmdList.SetUnorderedAccessViewCS(0, MeshVertexUAV);
    cmdList.SetUnorderedAccessViewCS(1, CraterBufferUAV);
    cmdList.SetUnorderedAccessViewCS(2, IntermediateNormalUAV);

    cmdList.SetConstantBufferViewCS(0, OceanNoiseParams->GetView());
    cmdList.SetConstantBufferViewCS(1, ContinentNoiseParams->GetView());
    cmdList.SetConstantBufferViewCS(2, MountainMaskNoiseParams->GetView());
    cmdList.SetConstantBufferViewCS(3, CraterParams->GetView());

    cmdList.DispatchCompute((mesh.VertexBuffer->VertexCount + 255) / 256, 1, 1);

    // Quantize and Accumulate normals
    cmdList.SetPipelineState(RenormalizeCompute);
    cmdList.SetUnorderedAccessViewCS(0, MeshVertexUAV);
    cmdList.SetUnorderedAccessViewCS(1, IntermediateNormalUAV);
    cmdList.SetUnorderedAccessViewCS(2, MeshIndexUAV);
    cmdList.DispatchCompute((mesh.IndexBuffer->IndexCount + 255) / 256, 1, 1);

    // Unquantize and finalize normals
    cmdList.SetPipelineState(RenormalizeCompute2);
    cmdList.SetUnorderedAccessViewCS(0, MeshVertexUAV);
    cmdList.SetUnorderedAccessViewCS(1, IntermediateNormalUAV);
    cmdList.SetUnorderedAccessViewCS(2, MeshIndexUAV);
    cmdList.DispatchCompute((mesh.VertexBuffer->VertexCount + 255) / 256, 1, 1);

    cmdList.EndCompute();
    //cmdList.CloseAndExecute();

}

void Planet::AddToCmdList(FRICommandList& cmdList, GRenderMode renderMode)
{

    cmdList.SetPipelineState(PlanetRender);
    for (uint32 layerIdx = 0; layerIdx < (uint32)EMaterialLayer::NUM_LAYERS; layerIdx++)
    {
        auto layerView = material.GetLayer((EMaterialLayer)layerIdx).View;
        auto layerSampler = material.GetLayer((EMaterialLayer)layerIdx).Sampler;
        if (layerView)//&& layerSampler)
        {
            cmdList.SetShaderResourceViewPS(layerIdx, layerView);
            //cmdList.SetShaderSamplerPS(layerIdx, layerSampler);
        }
    }

    mesh.AddToRenderList(cmdList, EFRIPrimitiveType::Triangles);
}