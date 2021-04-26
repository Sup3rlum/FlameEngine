#include "SATGenerator.h"


void SATGenerator::CreateResources(FRIContext* renderContext)
{
	FRICommandList cmdList(renderContext->GetFRIDynamic());


	FAnsiString vsData = IOFileStream("shaders/util/SATGen.vert").ReadAnsiFile();
	FAnsiString psData = IOFileStream("shaders/util/SATGen.frag").ReadAnsiFile();

	FResourceVertexShader* vs = cmdList.GetDynamic()->DynamicCreateVertexShader(vsData);
	FResourcePixelShader* ps = cmdList.GetDynamic()->DynamicCreatePixelShader(psData);


	pipeline = cmdList.GetDynamic()->DynamicCreateShaderPipeline
	(
		FResourceShaderPipelineCreationDescriptor(
			2,
			new FResourceShaderBase * [2]{ vs, ps }
		)
	);

	/* Create Texture2DArray */
	FResourceTextureColorDescriptor shadowDescriptor(EFRITextureChannelStorage::RG32F, EFRITextureChannels::RGBA, EFRITexturePixelStorage::Float);
	textureRefB = cmdList.GetDynamic()->DynamicCreateTexture2DArray(4096, 4096, 5, shadowDescriptor);


	/* Create parameter buffer */
	FArray<FTextureParameterBufferParameter> paramBuffer1;
	paramBuffer1.Add(FTextureParameterBufferParameter(EFRITextureParamName::MinFilter, EFRITextureFilterMode::Trilinear));
	paramBuffer1.Add(FTextureParameterBufferParameter(EFRITextureParamName::MagFilter, EFRITextureFilterMode::Trilinear));
	paramBuffer1.Add(FTextureParameterBufferParameter(EFRITextureParamName::AnisotropyLevel, 16.0f));

	cmdList.SetTextureParameterBuffer(textureRefB, FResourceTextureParameterBuffer(paramBuffer1));


	/* Create FBO */

	FArray<FResourceFrameBufferTextureAttachment> shadowmapAttachment;
	shadowmapAttachment.Add(FResourceFrameBufferTextureAttachment(textureRefB, EResourceFBTextureAttachmentType::Color));
	backFBO = cmdList.GetDynamic()->DynamicCreateFrameBuffer(shadowmapAttachment, true);

	PassIndexLoc = cmdList.GetDynamic()->GetShaderUniformParameterLocation(pipeline, "PassIndex");
	DirectionLoc = cmdList.GetDynamic()->GetShaderUniformParameterLocation(pipeline, "SumDirection");
	LayerIndexLoc = cmdList.GetDynamic()->GetShaderUniformParameterLocation(pipeline, "LayerIndex");



}

void SATGenerator::SetupContext(FVector2 texSize, uint32 numLayers)
{
	this->texSize = texSize;

	passDimensions.x = FMathFunc::Log4(texSize.x);
	passDimensions.y = FMathFunc::Log4(texSize.y);

	arraylayers = numLayers;
}


void SATGenerator::SwapTargets()
{
	auto temp = textureRefA;
	textureRefA = textureRefB;
	textureRefB = temp;

	auto temp2 = attachedFBO;
	attachedFBO = backFBO;
	backFBO = temp2;
}


void SATGenerator::RenderPass(FRICommandList& cmdList, FVector2 direction, float passIndex)
{
	cmdList.ClearBuffer(backFBO, Color::Transparent);
	{
		cmdList.SetShaderSampler(FUniformSampler(0, textureRefA));


		cmdList.SetShaderUniformParameter(FUniformParameter(PassIndexLoc, passIndex));
		cmdList.SetShaderUniformParameter(FUniformParameter(DirectionLoc, direction));

		for (uint32 i = 0; i < arraylayers; i++)
		{
			cmdList.SetShaderUniformParameter(FUniformParameter(LayerIndexLoc, (float)i));

			cmdList.SetFramebufferTextureLayer(textureRefA, i);
			cmdList.ClearBuffer(NULL, Color::Transparent);


			cmdList.SetGeometrySource(FScreenQuadUtil::VertexBuffer);
			cmdList.DrawPrimitivesIndexed((uint32)EFRIPrimitiveType::Triangles, 6, (uint32)EFRIIndexType::UInt32, FScreenQuadUtil::IndexBuffer);
		}

	}
	cmdList.UnbindFrameBuffer();
}

void SATGenerator::Render(FRICommandList& cmdList, FResourceTexture2DArray* textureArray, FResourceFrameBuffer* attachedFBO)
{
	this->textureRefA = textureArray;
	this->attachedFBO = attachedFBO;


	cmdList.SetShaderPipeline(pipeline);

	FVector2 horizontal(1, 0);
	FVector2 vertical(0, 1);

	for (uint32 i = 0; i < passDimensions.x; i++)
	{
		RenderPass(cmdList, horizontal, i);
		SwapTargets();
	}

	for (uint32 i = 0; i < passDimensions.x; i++)
	{
		RenderPass(cmdList, vertical, i);
		SwapTargets();
	}



}