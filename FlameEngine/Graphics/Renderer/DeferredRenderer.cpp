#include "DeferredRenderer.h"


DeferredRenderer::DeferredRenderer(Context* context)
{
	attachedContext = context;

	mBlurRenderingService = new KawaseBlurService(context, true);
	mHbaoPlusService = new HBAOPlusService(context);
	mBoundingService = new BoundingVolumeDebugService(context);

	View = fMatrix4::CreateOrthographic(0.0f, (float)context->_contextDescription->width, (float)context->_contextDescription->height, 0.0f, 0.0f, 1.0f);

	VertexTexture mQuadData[4] =
	{
		VertexTexture(fVector3(0,0,0), fVector2(0,1)),
		VertexTexture(fVector3(1,0,0), fVector2(1,1)),
		VertexTexture(fVector3(1,1,0), fVector2(1,0)),
		VertexTexture(fVector3(0,1,0), fVector2(0,0))
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


	mSceneDataShader	= Shader::FromSource(".\\shaders\\ssao_geom.vert", ".\\shaders\\ssao_geom.frag");
	mSsaoShader			= Shader::FromSource(".\\shaders\\renderbatch.vert", ".\\shaders\\ssao.frag");
	mCombineShader		= Shader::FromSource(".\\shaders\\renderbatch.vert", ".\\shaders\\ssao_final.frag");



	mDepthBuffer = new Texture(2560, 1440, GL_RG32F, GL_RGBA, GL_FLOAT, false);
	mDepthBuffer->SetFilteringMode(TextureFiltering::ANISOTROPIC_8);
	mDepthBuffer->SetWrappingMode(TextureWrapping::REPEAT);

	mNormalBuffer = new Texture(2560, 1440, GL_RGBA32F, GL_RGBA, GL_FLOAT, false);
	mNormalBuffer->SetFilteringMode(TextureFiltering::BILINEAR);
	mNormalBuffer->SetWrappingMode(TextureWrapping::REPEAT);

	mAlbedoBuffer = new Texture(2560, 1440, GL_RGBA32F, GL_RGBA, GL_FLOAT, false);
	mAlbedoBuffer->SetFilteringMode(TextureFiltering::BILINEAR);
	mAlbedoBuffer->SetWrappingMode(TextureWrapping::REPEAT);



	mSsaoBuffer = new Texture(2560, 1440, GL_RGBA32F, GL_RGBA, GL_FLOAT, false);
	mSsaoBuffer->SetFilteringMode(TextureFiltering::BILINEAR);
	mSsaoBuffer->SetWrappingMode(TextureWrapping::REPEAT);


	mShadowmapBuffer = new Texture(2560, 1440, GL_RGBA32F, GL_RGBA, GL_FLOAT, true);
	mShadowmapBuffer->SetFilteringMode(TextureFiltering::BILINEAR);
	mShadowmapBuffer->SetWrappingMode(TextureWrapping::REPEAT);



	mFrameBuffer = new FrameBuffer(2560, 1440);
	mFrameBuffer->Bind();
	mFrameBuffer->BindTexture(mDepthBuffer, GL_COLOR_ATTACHMENT0);
	mFrameBuffer->BindTexture(mNormalBuffer, GL_COLOR_ATTACHMENT1);
	mFrameBuffer->BindTexture(mAlbedoBuffer, GL_COLOR_ATTACHMENT2);
	mFrameBuffer->EnableDepth();
	mFrameBuffer->SetAttachments(new GLuint[3]{ GL_COLOR_ATTACHMENT0 ,GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 }, 3);
	mFrameBuffer->Unbind();


	mShadowmapFrameBuffer = new FrameBuffer(2560, 1440);
	mShadowmapFrameBuffer->Bind();
	mShadowmapFrameBuffer->BindTexture(mShadowmapBuffer, GL_COLOR_ATTACHMENT0);
	mShadowmapFrameBuffer->EnableDepth();
	mShadowmapFrameBuffer->Unbind();


	mSsaoFrameBuffer = new FrameBuffer(2560, 1440);
	mSsaoFrameBuffer->Bind();
	mSsaoFrameBuffer->BindTexture(mSsaoBuffer, GL_COLOR_ATTACHMENT0);
	//mS_ssaoFrameBuffer->EnableDepth();
	mSsaoFrameBuffer->Unbind();

}


void DeferredRenderer::BeginRender(Scene* scene)
{
	mShadowmapFrameBuffer->Bind();
	{
		OpenGL::Clear(Color::Transparent);

		scene->PushCamera(scene->DirectionalLightCollection[0].LightCamera());
		scene->Render();
		scene->PopCamera();
	}
	mShadowmapFrameBuffer->Unbind();

	mBlurRenderingService->ApplyFilter(mShadowmapFrameBuffer, mShadowmapBuffer, mBlurPassIndex);


	mFrameBuffer->Bind();
	{
		OpenGL::Clear(Color::Transparent);

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
	mCombineShader->SetMatrix("InverseProjection", scene->CurrentCamera()->ProjectionInverse);
	mCombineShader->SetMatrix("InverseView", scene->CurrentCamera()->ViewInverse);


	for (int i = 0; i < MAX_DIRECTIONAL_LIGHTS && i < scene->DirectionalLightCollection.size(); i++)
	{
		mCombineShader->SetVector("DirectionalLights[" + to_string(i) + "].Direction",	scene->DirectionalLightCollection[0].Direction);
		mCombineShader->SetVector("DirectionalLights[" + to_string(i) + "].Color",		scene->DirectionalLightCollection[0].LightColor);
		mCombineShader->SetFloat( "DirectionalLights[" + to_string(i) + "].Intensity",	scene->DirectionalLightCollection[0].Intensity);
		mCombineShader->SetMatrix("DirectionalLights[" + to_string(i) + "].VPMatrix",	scene->DirectionalLightCollection[0].LightCamera()->Projection * scene->DirectionalLightCollection[0].LightCamera()->View);
	}

	mCombineShader->SetInt("gDepth", 0);
	mCombineShader->SetInt("gNormal", 1);
	mCombineShader->SetInt("gAlbedo", 2);
	mCombineShader->SetInt("ssao", 3);
	mCombineShader->SetInt("_shadowMap", 4);

	mCombineShader->SetMatrix("View", View);
	mCombineShader->SetMatrix("MatrixTransforms", fMatrix4::Scaling(fVector3(attachedContext->_contextDescription->width, attachedContext->_contextDescription->height, 1)));


	mCombineShader->SetTexture(0, mDepthBuffer);
	mCombineShader->SetTexture(1, mNormalBuffer);
	mCombineShader->SetTexture(2, mAlbedoBuffer);
	mCombineShader->SetTexture(3, mSsaoBuffer);
	mCombineShader->SetTexture(4, mShadowmapBuffer);


	mQuadBuffer->RenderIndexed(GL_TRIANGLES);

	RenderState::Pop();

	/*
	fVector3 corn[8];

	scene->CurrentCamera()->GetFrustumCorners(corn);
	mBoundingService->Render(scene->CurrentCamera(), corn);

	/*scene->LightCollection[0].aabb.GetCorners(corn);
	for (int i = 0; i < 8; i++)
	{
		corn[i] = fMatrix3::Transpose(scene->LightCollection[0].localSpace) * corn[i];
	}
	mBoundingService->Render(scene->CurrentCamera(), corn);

	fVector3 ndc_corners[8] =
	{
		fVector3(1.0f, -1.0f, -1.0f),	 // llb
		fVector3(-1.0f, -1.0f, -1.0f), // lrb
		fVector3(-1.0f, 1.0f, -1.0f),  // urb
		fVector3(1.0f, 1.0f, -1.0f),	 // ulb


		fVector3(1.0f, -1.0f, 1.0f),   // llf
		fVector3(-1.0f, -1.0f, 1.0f),  // lrf
		fVector3(-1.0f, 1.0f, 1.0f),   // urf
		fVector3(1.0f, 1.0f, 1.0f)	 // ulf

	};

	fVector3 lightPos = fMatrix3::Transpose(scene->LightCollection[0].localSpace) * (scene->LightCollection[0].aabb.Center() + fVector3(0, 0, scene->LightCollection[0].aabb.LengthZ() / 2.0f));

	for (int i = 0; i < 8; i++)
	{
		corn[i] = (fMatrix4::Transpose(fMatrix4::Translation(lightPos)) * fVector4(ndc_corners[i] * 0.5f, 1.0f)).xyz;
	}
	mBoundingService->Render(scene->CurrentCamera(), corn);

	scene->LightCollection[0]._cam.GetFrustumCorners(corn);

	mBoundingService->Render(scene->CurrentCamera(), corn);*/

	/*
	if (enableDEBUGTexture)
	{
		scene->_renderBatch->DrawTexture(mDepthBuffer, 1200, 0, 200, 200);
		scene->_renderBatch->DrawTexture(mNormalBuffer, 1400, 0, 200, 200);
		scene->_renderBatch->DrawTexture(mAlbedoBuffer, 1600, 0, 200, 200);
		scene->_renderBatch->DrawTexture(mSsaoBuffer, 1800, 0, 200, 200);
		scene->_renderBatch->DrawTexture(mShadowmapBuffer, 2000, 0, 200, 200);
	}*/
	scene->pUxService->Render();
}

void DeferredRenderer::EndRender()
{

}