#include "Spine.h"
#include <FlameEngine/Core/Engine/Renderer/Common/RenderUtil.h>
#include <FlameEngine/Core/Framework/IO/FileStream.h>

const float M_PI = 3.141592;
const int MAX_CRATERS = 1024;



FRIPipelineStateObject* Spine::CreateGeometryPipelineState(const ShaderLibraryModule& Shaders)
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
        .NumElements = 2,
        .DeclarationElements = new FRIInputElementDesc[2]
        {
            FRIInputElementDesc("POSITION", EFRIAttributeType::Float3, 0, EFRIAttribUsage::PerVertex),
            FRIInputElementDesc("TEXCOORD", EFRIAttributeType::Float2, 1, EFRIAttribUsage::PerVertex)
        }
    };

    // Texture 0

    FRIDescriptorRange Texture0Range;
    Texture0Range.NumDescriptors = 1;
    Texture0Range.BaseShaderRegister = 0;
    Texture0Range.RegisterSpace = 0;
    Texture0Range.OffsetInDescriptorsFromTableStart = 0xffffffff;
    Texture0Range.RangeType = EFRIRootDescriptorRangeType::SRV;

    FRIRootDescriptorTable Textures;
    Textures.NumRanges = 1;
    Textures.Ranges = &Texture0Range;

    // CBV

    FRIDescriptorRange ConstantBuffer1Range;
    ConstantBuffer1Range.NumDescriptors = 1;
    ConstantBuffer1Range.BaseShaderRegister = 0;
    ConstantBuffer1Range.RegisterSpace = 0;
    ConstantBuffer1Range.OffsetInDescriptorsFromTableStart = 0xffffffff;
    ConstantBuffer1Range.RangeType = EFRIRootDescriptorRangeType::CBV;

    FRIRootDescriptorTable ConstantBufferTable;
    ConstantBufferTable.NumRanges = 1;
    ConstantBufferTable.Ranges = &ConstantBuffer1Range;

    FArray<FRIRootParameter> RootParams = {
            FRIRootParameter(EFRIRootParameterType::DESCRIPTOR_TABLE, EFRIShaderVisibility::Pixel, ConstantBufferTable),
            FRIRootParameter(EFRIRootParameterType::DESCRIPTOR_TABLE, EFRIShaderVisibility::Pixel, Textures),
    };
    FArray<FRIStaticSampler> StaticSamplers = {
            FRIStaticSampler(EFRITextureFilter::Bilinear, EFRITextureAddress::Repeat, EFRITextureAddress::Repeat, EFRITextureAddress::Repeat, 0, 0),
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


FRITexture3D* CreateSDFFromPointCloud(FRIContext* FriContext, FArray<FVector3> pts, int size)
{
    auto allocator = FriContext->GetFRIDynamic();

    FArray<float> out;
    for (int x = 0; x < size; x++)
    {
        for (int y = 0; y < size; y++)
        {
            for (int z = 0; z < size; z++)
            {
                
                float minDist = 999999999.0f;
                for (auto p : pts)
                {
                    float dist = (p - FVector3(x, y, z)).Length();
                    minDist = FMath::Min(dist, minDist);
                }

                out.Add(minDist - 3.0f);
                printf("Computing: %d %d %d\n", x, y, z);
            }
        }
    }

    return allocator->CreateTexture3D(size, size, size, 1, EFRIAccess::None, EFRITextureFormat::R32F, new FRICreationDescriptor(out.Begin(), out.ByteSize()));
}


FRITexture3D* LoadSDF(FRIContext* FriContext, const FString& path)
{
    auto allocator = FriContext->GetFRIDynamic();

    IOFileStream fileStream(path);
    int nx = fileStream.Read<int>();
    int ny = fileStream.Read<int>();
    int nz = fileStream.Read<int>(); 
    auto sdf = fileStream.ReadArray<float>(nx * ny * nz);

    return allocator->CreateTexture3D(nx, ny, nz, 1, EFRIAccess::None, EFRITextureFormat::R32F, new FRICreationDescriptor(sdf.Begin(), sdf.ByteSize()));
}

Spine::Spine(FRIContext* FriContext) :
	FriContext(FriContext)
{
	auto Allocator = FriContext->GetFRIDynamic();
	FAssetManager Content;
	Content.RenderContext = FriContext;
	Content.Connect("./Assets/");

    material = Content.Load<Material>("Materials/default2.flmt");
	auto planetaryShaders = Content.Load<ShaderLibrary>("Shaders/spinal_dx.fslib");

    FArray<FVector3> pts;

    for (int i = 0; i < 10; i++)
    {
        for (int j = 0; j < 10; j++)
        {
            pts.Add(FVector3(40, i * 3 + 10, j * 3 + 10));
            pts.Add(FVector3(50, i * 3 + 10, j * 3 + 10));
        }
    }

    FRICommandList cmdList(FriContext->GetCommandContext(0));

    cmdList.Open();
    SpineRender = CreateGeometryPipelineState(planetaryShaders.Modules["SpineRayMarch"]);
    SDF = LoadSDF(FriContext, "sdfvox.bin");
    SDF->Rename("SDF");
    SDFsrv = Allocator->CreateShaderResourceView(SDF);
    cmdList.CloseAndExecute();

}

void Spine::AddToCmdList(FRICommandList& cmdList, GRenderMode renderMode)
{

    cmdList.SetPipelineState(SpineRender);
    cmdList.SetShaderResourceViewPS(0, SDFsrv);

    FRenderUtil::DrawScreenQuad(cmdList);
}