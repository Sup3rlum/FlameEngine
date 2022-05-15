#include "RenderModel.h"
#include "ImportScripts/BitTexture.h"

ModelRenderObject::ModelRenderObject(FRIContext* FriContext) :
	FriContext(FriContext)
{
	FRICommandList cmdList(FriContext->GetFRIDynamic());


	FAssetManager Content;
	Content.RenderContext = FriContext;
	Content.Connect("./Assets/");


	ShaderLibrary lib = Content.Load<ShaderLibrary>("Shaders/showcase.fslib");
	dissolveShader = cmdList.GetDynamic()->CreateShaderPipeline(lib.Modules["Dissolve"]);

	dissolveBuffer.GPU = cmdList.GetDynamic()->CreateUniformBuffer(FRICreationDescriptor(NULL, 16));

	BitTexture bitTexture = Content.Load<BitTexture>("Materials/dissolve_mask.flmt");
	dissolveMask = cmdList.GetDynamic()->CreateTexture2D(256, 256, 0, EFRITextureFormat::RGBA8UNORM, FRIColorDataFormat(EFRIChannels::RGBA, EFRIPixelStorage::UnsignedByte), bitTexture.DataDescriptor);

	rasterizer = cmdList.GetDynamic()->CreateRasterizerState(EFRICullMode::None, EFRIFillMode::Solid);

}

void ModelRenderObject::AddToCmdList(FRICommandList& cmdList, GRenderMode renderMode)
{
	if (renderMode == GRenderMode::Transluscent)
		return;

	cmdList.SetShaderPipeline(dissolveShader);
	cmdList.StageResourcesLambda(dissolveBuffer, [&](FRIMemoryMap& memory)
		{
			auto time = FTime::GetTimestamp();
			memory << time.GetSeconds();
			memory << time.GetSeconds();
			memory << time.GetSeconds();
			memory << time.GetSeconds();
		});

	cmdList.SetShaderUniformBuffer(12, dissolveBuffer.GPU);
	cmdList.SetShaderSampler(FUniformSampler(4, dissolveMask));
	cmdList.SetRasterizerState(rasterizer);


	/* Stage Samplers */
	for (int map = 0; map < (uint32)EMaterialMap::MAX_MAPS; map++)
	{
		FRITexture2D* textureMap = model.Material.GetMap((EMaterialMap)map).Handle;
		if (textureMap)
		{
			cmdList.SetShaderSampler(FUniformSampler(map, textureMap));
		}
	}

	model.Mesh.AddToRenderList(cmdList, EFRIPrimitiveType::Triangles);
}
