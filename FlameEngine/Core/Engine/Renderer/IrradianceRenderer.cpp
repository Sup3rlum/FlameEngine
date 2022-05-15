#include "IrradianceRenderer.h"

/*


void DeferredRenderer::RenderEnvMap(FRICommandList& cmdList, FVector3 vec)
{

	FPerspectiveMatrix projection(PI / 4, 1.0f, 0.1f, 200.0f);

	Camera faceCameras[] = {
		Camera(FViewMatrix(vec, vec + FVector3(-1,0,0), FVector3(0,1,0)), projection),
		Camera(FViewMatrix(vec, vec + FVector3(1,0,0),	FVector3(0,1,0)), projection),
		Camera(FViewMatrix(vec, vec + FVector3(0,1,0),	FVector3(0,0,1)), projection),
		Camera(FViewMatrix(vec, vec + FVector3(0,-1,0), FVector3(0,0,1)), projection),
		Camera(FViewMatrix(vec, vec + FVector3(0,0,1),	FVector3(0,1,0)), projection),
		Camera(FViewMatrix(vec, vec + FVector3(0,0,-1), FVector3(0,1,0)), projection),
	};
}

FRITextureCubeMap* DeferredRenderer::RenderIrradiance(FRIContext* renderContext, FRICreationDescriptor* data)
{
	FRICommandList cmdList(renderContext->GetFRIDynamic());

	FPerspectiveMatrix persp(PI / 4.0f, 1, 0.1f, 20.0f);
	FViewMatrix captureViews[] =
	{
		FViewMatrix(FVector3(0.0f, 0.0f, 0.0f), FVector3(1.0f,  0.0f,  0.0f), FVector3(0.0f, -1.0f,  0.0f)),
		FViewMatrix(FVector3(0.0f, 0.0f, 0.0f), FVector3(-1.0f,  0.0f,  0.0f),FVector3(0.0f, -1.0f,  0.0f)),
		FViewMatrix(FVector3(0.0f, 0.0f, 0.0f), FVector3(0.0f,  1.0f,  0.0f), FVector3(0.0f,  0.0f,  1.0f)),
		FViewMatrix(FVector3(0.0f, 0.0f, 0.0f), FVector3(0.0f, -1.0f,  0.0f), FVector3(0.0f,  0.0f, -1.0f)),
		FViewMatrix(FVector3(0.0f, 0.0f, 0.0f), FVector3(0.0f,  0.0f,  1.0f), FVector3(0.0f, -1.0f,  0.0f)),
		FViewMatrix(FVector3(0.0f, 0.0f, 0.0f), FVector3(0.0f,  0.0f, -1.0f), FVector3(0.0f, -1.0f,  0.0f))
	};

	FAssetManager Content;
	Content.RenderContext = renderContext;
	Content.Connect("./Assets/");

	ShaderLibrary shaderLib = Content.Load<ShaderLibrary>("Shaders/irradiance.fslib");
	auto irrShader = cmdList.GetDynamic()->CreateShaderPipeline(shaderLib.Modules["Irradiance"]);

	auto mesh = Content.Load<Mesh>("Models/cube.fl3d");

	auto cubeMap = cmdList.GetDynamic()->CreateTextureCubeMap(
		256,
		256,
		1,
		EFRITextureFormat::RGBA8UNORM,
		FRIColorDataFormat(EFRIChannels::RGBA, EFRIPixelStorage::UnsignedByte),
		data);

	auto result = cmdList.GetDynamic()->CreateTextureCubeMap(
		256,
		256,
		1,
		EFRITextureFormat::RGBA8UNORM);

	FRIStageBuffer uniBuff;

	auto frameBuffer = cmdList.GetDynamic()->CreateFrameBuffer(result, true, 0);
	uniBuff.GPU = cmdList.GetDynamic()->CreateUniformBuffer(FRICreationDescriptor(NULL, sizeof(FMatrix4) * 2));


	cmdList.SetShaderUniformBuffer(0, uniBuff.GPU);
	cmdList.ClearBuffer(frameBuffer, Color::Black);
	{
		cmdList.SetShaderPipeline(irrShader);

		for (int i = 0; i < 6; i++)
		{
			cmdList.SetFramebufferTextureLayer(frameBuffer, i);
			cmdList.StageResourcesLambda(uniBuff, [=](FRIMemoryMap& stageMemory)
				{
					stageMemory << captureViews[i];
					stageMemory << persp;
				});

			cmdList.SetShaderSampler(FUniformSampler(0, cubeMap));


			cmdList.SetGeometrySource(mesh.VertexBuffer);
			cmdList.DrawPrimitivesIndexed(EFRIPrimitiveType::Triangles, mesh.IndexBuffer->IndexCount, EFRIIndexType::UInt32, mesh.IndexBuffer);

		}
	}
	cmdList.UnbindFrameBuffer();

	return result;
}




void DeferredRenderer::RenderIrradiance2(FRICommandList& cmdList, FRITextureCubeMap* data)
{
	cmdList.SetRasterizerState(SMRasterizer);
	cmdList.SetBlendState(DefaultBlend);
	cmdList.SetDepthStencilState(DisableDepth);


	FPerspectiveMatrix persp(PI / 4.0f, 1, 0.1f, 20.0f);
	FViewMatrix captureViews[] =
	{
		FViewMatrix(FVector3(0.0f, 0.0f, 0.0f), FVector3(1.0f,  0.0f,  0.0f), FVector3(0.0f, -1.0f,  0.0f)),
		FViewMatrix(FVector3(0.0f, 0.0f, 0.0f), FVector3(-1.0f,  0.0f,  0.0f),FVector3(0.0f, -1.0f,  0.0f)),
		FViewMatrix(FVector3(0.0f, 0.0f, 0.0f), FVector3(0.0f,  1.0f,  0.0f), FVector3(0.0f,  0.0f,  1.0f)),
		FViewMatrix(FVector3(0.0f, 0.0f, 0.0f), FVector3(0.0f, -1.0f,  0.0f), FVector3(0.0f,  0.0f, -1.0f)),
		FViewMatrix(FVector3(0.0f, 0.0f, 0.0f), FVector3(0.0f,  0.0f,  1.0f), FVector3(0.0f, -1.0f,  0.0f)),
		FViewMatrix(FVector3(0.0f, 0.0f, 0.0f), FVector3(0.0f,  0.0f, -1.0f), FVector3(0.0f, -1.0f,  0.0f))
	};

	cmdList.SetViewport(FViewportRect(0, 0, 256, 256));

	cmdList.SetShaderUniformBuffer(0, uniBuff.GPU);
	cmdList.ClearBuffer(irrFrameBuffer, Color::Black);
	{
		cmdList.SetShaderPipeline(irrShader);

		for (int i = 0; i < 6; i++)
		{
			cmdList.SetFramebufferTextureLayer(irrFrameBuffer, i);
			cmdList.StageResourcesLambda(uniBuff, [=](FRIMemoryMap& stageMemory)
				{
					stageMemory << captureViews[i];
					stageMemory << persp;
				});

			cmdList.SetShaderSampler(FUniformSampler(0, data));


			cmdList.SetGeometrySource(mesh.VertexBuffer);
			cmdList.DrawPrimitivesIndexed(EFRIPrimitiveType::Triangles, mesh.IndexBuffer->IndexCount, EFRIIndexType::UInt32, mesh.IndexBuffer);

		}
	}
	cmdList.UnbindFrameBuffer();

	//scene->Env.Component<EnvironmentMap>().Irradiance = result;
}*/