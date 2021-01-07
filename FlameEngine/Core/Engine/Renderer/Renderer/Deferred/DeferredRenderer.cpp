#include "DeferredRenderer.h"


DeferredRenderer::DeferredRenderer(Context* context)
{
	attachedContext = context;

	mBlurRenderingService = new KawaseBlurService(context, true);
	mHbaoPlusService = new HBAOPlusService(context);
	mBoundingService = new BoundingVolumeDebugService(context);

	View = FMatrix4::CreateOrthographic(0.0f, (float)context->_contextDescription->width, (float)context->_contextDescription->height, 0.0f, 0.0f, 1.0f);
	//View = FMatrix4::CreateOrthographic(0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f);

	VertexTexture mQuadData[4] =
	{
		VertexTexture(FVector3(0,0,0), FVector2(0,1)),
		VertexTexture(FVector3(1,0,0), FVector2(1,1)),
		VertexTexture(FVector3(1,1,0), FVector2(1,0)),
		VertexTexture(FVector3(0,1,0), FVector2(0,0))
	};
	GLuint mQuadIndexData[6]
	{
		0,
		1,
		2,

		0,
		2,
		3
	};



	mQuadBuffer = new VertexBuffer(VertexTexture::Elements);
	mQuadBuffer->SetIndexedData<VertexTexture>(mQuadData, mQuadIndexData, 4, 6);

	mRenderState = new RenderState();
	mRenderState->CullState = CullState::Front;
	mRenderState->DepthFunction = DepthFunc::Always;
	mRenderState->SourceBlend = BlendFunc::SourceAlpha;
	mRenderState->DestinationBlend = BlendFunc::OneMinusSourceAlpha;


	Shader* geomVert = FLSLCompilerService::CompileShaderFromSourceFile(".\\shaders\\ssao_geom.vert", ShaderType::VERTEX);
	Shader* quadCommonVert = FLSLCompilerService::CompileShaderFromSourceFile(".\\shaders\\renderbatch.vert", ShaderType::VERTEX);

	Shader* geomFrag = FLSLCompilerService::CompileShaderFromSourceFile(".\\shaders\\ssao_geom.frag", ShaderType::FRAGMENT);
	Shader* ssaoFrag = FLSLCompilerService::CompileShaderFromSourceFile(".\\shaders\\ssao.frag", ShaderType::FRAGMENT);
	Shader* finalFrag = FLSLCompilerService::CompileShaderFromSourceFile(".\\shaders\\ssao_final.frag", ShaderType::FRAGMENT);



	Shader* sceneshaders[2] =
	{
		geomVert,
		geomFrag
	};

	Shader* ssaoshaders[2] =
	{
		quadCommonVert,
		ssaoFrag
	};

	Shader* combineshaders[2] =
	{
		quadCommonVert,
		finalFrag
	};

	mSceneDataShader	= new Program(sceneshaders);
	mCombineShader		= new Program(combineshaders);



	// G-Buffer

	mDepthBuffer = new Texture(2560, 1440, GL_RG32F, GL_RGBA, GL_FLOAT);
	mDepthBuffer->SetFilteringMode(TextureFiltering::BILINEAR);
	mDepthBuffer->SetWrappingMode(TextureWrapping::REPEAT);

	mNormalBuffer = new Texture(2560, 1440, GL_RGBA32F, GL_RGBA, GL_FLOAT);
	mNormalBuffer->SetFilteringMode(TextureFiltering::BILINEAR);
	mNormalBuffer->SetWrappingMode(TextureWrapping::REPEAT);

	mAlbedoBuffer = new Texture(2560, 1440, GL_RGBA32F, GL_RGBA, GL_FLOAT);
	mAlbedoBuffer->SetFilteringMode(TextureFiltering::BILINEAR);
	mAlbedoBuffer->SetWrappingMode(TextureWrapping::REPEAT);

	mSpecularBuffer = new Texture(2560, 1440, GL_RGBA32F, GL_RGBA, GL_FLOAT);
	mSpecularBuffer->SetFilteringMode(TextureFiltering::BILINEAR);
	mSpecularBuffer->SetWrappingMode(TextureWrapping::REPEAT);



	mSsaoBuffer = new Texture(2560, 1440, GL_RGBA32F, GL_RGBA, GL_FLOAT);
	mSsaoBuffer->SetFilteringMode(TextureFiltering::BILINEAR);
	mSsaoBuffer->SetWrappingMode(TextureWrapping::REPEAT);


	mSsaoFrameBuffer = new FrameBuffer(2560, 1440);
	mSsaoFrameBuffer->Bind();
	mSsaoFrameBuffer->BindTexture(mSsaoBuffer, GL_COLOR_ATTACHMENT0);
	mSsaoFrameBuffer->Unbind();


	mFrameBuffer = new FrameBuffer(2560, 1440);
	mFrameBuffer->Bind();
	mFrameBuffer->BindTexture(mDepthBuffer, GL_COLOR_ATTACHMENT0);
	mFrameBuffer->BindTexture(mNormalBuffer, GL_COLOR_ATTACHMENT1);
	mFrameBuffer->BindTexture(mAlbedoBuffer, GL_COLOR_ATTACHMENT2);
	mFrameBuffer->BindTexture(mSpecularBuffer, GL_COLOR_ATTACHMENT3);
	mFrameBuffer->EnableDepth();
	mFrameBuffer->SetAttachments(new GLuint[4]{ GL_COLOR_ATTACHMENT0 ,GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3 }, 4);
	mFrameBuffer->Unbind();

	
	mShadowmapBuffer = new Texture(4096, 4096, GL_RG32F, GL_RGBA, GL_FLOAT);
	mShadowmapBuffer->SetFilteringMode(TextureFiltering::BILINEAR);
	mShadowmapBuffer->SetWrappingMode(TextureWrapping::REPEAT);
	mShadowmapBuffer->SetAnisotropyLevel(4.0f);

	mShadowmapFrameBuffer = new FrameBuffer(4096, 4096);
	mShadowmapFrameBuffer->Bind();
	mShadowmapFrameBuffer->BindTexture(mShadowmapBuffer, GL_COLOR_ATTACHMENT0);
	mShadowmapFrameBuffer->EnableDepth();
	mShadowmapFrameBuffer->Unbind();


	

	_renderBatch = new RenderBatch(context);
}


void DeferredRenderer::BeginRender(Scene* scene)
{

	glViewport(0, 0, 4096, 4096);

	mShadowmapFrameBuffer->Bind();
	{
		mShadowmapFrameBuffer->Clear(Color::Transparent);


		mSceneDataShader->UseProgram();

		scene->PushCamera(scene->DirectionalLightCollection[0].LightCamera());
		scene->Render();
		scene->PopCamera();
	}
	mShadowmapFrameBuffer->Unbind();

	mBlurRenderingService->ApplyFilter(mShadowmapFrameBuffer, mShadowmapBuffer, mBlurPassIndex);

	glViewport(0, 0, 2560, 1440);




	mFrameBuffer->Bind();
	{
		mFrameBuffer->Clear(Color::Transparent);

		mSceneDataShader->UseProgram();

		scene->Render();
	}
	mFrameBuffer->Unbind();

	if (enableHBAO)
	{
		mHbaoPlusService->RenderAO(mDepthBuffer, mNormalBuffer, mSsaoFrameBuffer, scene->CurrentCamera()->Projection, scene->CurrentCamera()->View);

	}
	else
	{
		mSsaoFrameBuffer->Clear(Color::White);
	}

	OpenGL::Clear(Color::CornflowerBlue);

	RenderState::Push(mRenderState);

	mCombineShader->UseProgram();
	mCombineShader->SetUniform("InverseProjection", scene->CurrentCamera()->ProjectionInverse);
	mCombineShader->SetUniform("InverseView", scene->CurrentCamera()->ViewInverse);


	for (int i = 0; i < MAX_DIRECTIONAL_LIGHTS && i < scene->DirectionalLightCollection.size(); i++)
	{
		mCombineShader->SetUniform("DirectionalLights[" + to_string(i) + "].Direction",	scene->DirectionalLightCollection[i].Direction);
		mCombineShader->SetUniform("DirectionalLights[" + to_string(i) + "].Color",		scene->DirectionalLightCollection[i].LightColor);
		mCombineShader->SetUniform("DirectionalLights[" + to_string(i) + "].Intensity",	scene->DirectionalLightCollection[i].Intensity);
		mCombineShader->SetUniform("DirectionalLights[" + to_string(i) + "].VPMatrix",	scene->DirectionalLightCollection[i].LightCamera()->Projection * scene->DirectionalLightCollection[0].LightCamera()->View);
	}

	for (int i = 0; i < MAX_POINT_LIGHTS && i < scene->PointLightCollection.size(); i++)
	{
		mCombineShader->SetUniform("PointLights[" + to_string(i) + "].Position",			scene->PointLightCollection[i].Position);
		mCombineShader->SetUniform("PointLights[" + to_string(i) + "].Intensity",			scene->PointLightCollection[i].Intensity);
		mCombineShader->SetUniform("PointLights[" + to_string(i) + "].Radius",				scene->PointLightCollection[i].Radius);
		mCombineShader->SetUniform("PointLights[" + to_string(i) + "].Color",				scene->PointLightCollection[i].LightColor);
	}



	mCombineShader->SetUniform("NumLights", (int)scene->PointLightCollection.size());
	mCombineShader->SetUniform("CameraPosition", scene->CurrentCamera()->Position);

	mCombineShader->SetUniform("gDepth", 0);
	mCombineShader->SetUniform("gNormal", 1);
	mCombineShader->SetUniform("gAlbedo", 2);
	mCombineShader->SetUniform("gSpecular", 3);
	mCombineShader->SetUniform("ssao", 4);
	mCombineShader->SetUniform("_shadowMap", 5);

	mCombineShader->SetUniform("View", View);
	mCombineShader->SetUniform("MatrixTransforms", FMatrix4::Scaling(FVector3(attachedContext->_contextDescription->width, attachedContext->_contextDescription->height, 1)));


	mCombineShader->SetTexture(0, mDepthBuffer);
	mCombineShader->SetTexture(1, mNormalBuffer);
	mCombineShader->SetTexture(2, mAlbedoBuffer);
	mCombineShader->SetTexture(3, mSpecularBuffer);
	mCombineShader->SetTexture(4, mSsaoBuffer);
	mCombineShader->SetTexture(5, mShadowmapBuffer);

	

	mQuadBuffer->RenderIndexed(GL_TRIANGLES);

	RenderState::Pop();


	
	if (enableDEBUGTexture)
	{
		_renderBatch->DrawTexture(mDepthBuffer, 1200, 0, 200, 200);
		_renderBatch->DrawTexture(mNormalBuffer, 1400, 0, 200, 200);
		_renderBatch->DrawTexture(mAlbedoBuffer, 1600, 0, 200, 200);
		_renderBatch->DrawTexture(mSpecularBuffer, 1800, 0, 200, 200);
		_renderBatch->DrawTexture(mShadowmapBuffer, 2000, 0, 200, 200);
	}
	scene->pUxService->Render();
}

void DeferredRenderer::EndRender()
{
	
}
void DeferredRenderer::ScreenShot()
{
	BYTE* pixelData = Memory::Create<BYTE>(2560 * 1440 * 4);

	glReadPixels(0, 0, 2560, 1440, GL_RGBA, GL_UNSIGNED_BYTE, pixelData);

	PngHelper::SaveToDisk("test", 2560, 1440, pixelData);
}