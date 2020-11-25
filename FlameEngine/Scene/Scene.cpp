#include "Scene.h"


GLenum attachments[4] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 };




Scene::Scene(Context* _cont)
{
	_context = _cont;


	pPhysXService = new PhysXService();


	pUxService = new UxService();

	pUxContainer = new UxContainer(_cont);
	pUxService->PushContainer(pUxContainer);

	UxConsoleWindow* cons = new UxConsoleWindow();
	cons->Position = fVector2(1600, 300);
	cons->Size = fVector2(500, 500);

	pUxContainer->AddComponent(new UxDebugViewComponent(this));
	pUxContainer->AddComponent(cons);


	if (pPhysXService->InitializePhysX() == FLRESULT::FAIL)
	{
		printf("Something is wrong!\n");
	}

	pPxScene = pPhysXService->CreateScene();


	if (pPxScene == NULL)
	{
		printf("Scene is wrong!\n");
	}
	else
	{
		FLAME_INFO("PhysX Scene initialized");
	}

	PushCamera(new FpsCamera(_cont));

	gaussBlur = Shader::FromSource(".\\shaders\\renderbatch.vert", ".\\shaders\\gauss_blur.frag");
	_ssaoShader = Shader::FromSource(".\\shaders\\renderbatch.vert", ".\\shaders\\ssao.frag");
	_ssaoFinal = Shader::FromSource(".\\shaders\\renderbatch.vert", ".\\shaders\\ssao_final.frag");

	_tex = new Texture(".\\textures\\dirt.jpg");

	_depthMap = new Texture(2560, 1440, GL_RGB32F, GL_RGB, GL_FLOAT, true);
	_depthMap->Bind();
	_depthMap->SetParameter(GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	_depthMap->SetParameter(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	_depthMap->SetParameter(GL_TEXTURE_WRAP_S, GL_REPEAT);
	_depthMap->SetParameter(GL_TEXTURE_WRAP_T, GL_REPEAT);
	_depthMap->Unbind();



	_blurTexture = new Texture(2560, 1440, GL_RGB32F, GL_RGBA, GL_FLOAT, true);
	_blurTexture->Bind();
	_blurTexture->SetParameter(GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	_blurTexture->SetParameter(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	_blurTexture->SetParameter(GL_TEXTURE_WRAP_S, GL_REPEAT);
	_blurTexture->SetParameter(GL_TEXTURE_WRAP_T, GL_REPEAT);
	_blurTexture->Unbind();


	_frameBuffer = new FrameBuffer(2560, 1440);
	_frameBuffer->Bind();
	_frameBuffer->BindTexture(_depthMap, GL_COLOR_ATTACHMENT0);
	_frameBuffer->EnableDepth();
	_frameBuffer->Unbind();

	_blurFrameBuffer = new FrameBuffer(2560, 1440);
	_blurFrameBuffer->Bind();
	_blurFrameBuffer->BindTexture(_blurTexture, GL_COLOR_ATTACHMENT0);
	_blurFrameBuffer->Unbind();


	_depthRenderState = new RenderState();

	_depthRenderState->CullState = CullState::Back;
	_depthRenderState->DepthFunction = DepthFunc::LessOrEqual;


	_SkyRenderState = new RenderState();

	_SkyRenderState->CullState = CullState::Front;
	_SkyRenderState->DepthFunction = DepthFunc::Never;


	_gBuffer = new Texture(2560, 1440, GL_RG32F, GL_RGB, GL_FLOAT, false);
	_gBuffer->Bind();
	_gBuffer->SetParameter(GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	_gBuffer->SetParameter(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	_gBuffer->SetParameter(GL_TEXTURE_WRAP_S, GL_REPEAT);
	_gBuffer->SetParameter(GL_TEXTURE_WRAP_T, GL_REPEAT);
	_gBuffer->Unbind();

	_nBuffer = new Texture(2560, 1440, GL_RGB32F, GL_RGB, GL_FLOAT, false);
	_nBuffer->Bind();
	_nBuffer->SetParameter(GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	_nBuffer->SetParameter(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	_nBuffer->SetParameter(GL_TEXTURE_WRAP_S, GL_REPEAT);
	_nBuffer->SetParameter(GL_TEXTURE_WRAP_T, GL_REPEAT);
	_nBuffer->Unbind();

	_aBuffer = new Texture(2560, 1440, GL_RGB32F, GL_RGB, GL_FLOAT, false);
	_aBuffer->Bind();
	_aBuffer->SetParameter(GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	_aBuffer->SetParameter(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	_aBuffer->SetParameter(GL_TEXTURE_WRAP_S, GL_REPEAT);
	_aBuffer->SetParameter(GL_TEXTURE_WRAP_T, GL_REPEAT);
	_aBuffer->Unbind();



	_ssaoTexture = new Texture(2560, 1440, GL_RGB32F, GL_RGB, GL_FLOAT, false);
	_ssaoTexture->Bind();
	_ssaoTexture->SetParameter(GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	_ssaoTexture->SetParameter(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	_ssaoTexture->SetParameter(GL_TEXTURE_WRAP_S, GL_REPEAT);
	_ssaoTexture->SetParameter(GL_TEXTURE_WRAP_T, GL_REPEAT);
	_ssaoTexture->Unbind();




	_sceneFrameBuffer = new FrameBuffer(2560, 1440);
	_sceneFrameBuffer->Bind();
	_sceneFrameBuffer->BindTexture(_gBuffer, GL_COLOR_ATTACHMENT0);
	_sceneFrameBuffer->BindTexture(_nBuffer, GL_COLOR_ATTACHMENT1);
	_sceneFrameBuffer->BindTexture(_aBuffer, GL_COLOR_ATTACHMENT2);

	_sceneFrameBuffer->EnableDepth();

	_sceneFrameBuffer->SetAttachments(attachments, 3);

	_sceneFrameBuffer->Unbind();



	_ssaoFrameBuffer = new FrameBuffer(2560, 1440);
	_ssaoFrameBuffer->Bind();
	_ssaoFrameBuffer->BindTexture(_ssaoTexture, GL_COLOR_ATTACHMENT0);
	//_ssaoFrameBuffer->EnableDepth();
	_ssaoFrameBuffer->Unbind();


	std::uniform_real_distribution<GLfloat> randomFloats(0.0, 1.0); // generates random floats between 0.0 and 1.0
	std::default_random_engine generator;

	for (unsigned int i = 0; i < 64; ++i)
	{
		fVector3 sample(randomFloats(generator) * 2.0f - 1.0f, randomFloats(generator) * 2.0f - 1.0f, randomFloats(generator));
		sample = fVector3::Normalize(sample);
		sample *= randomFloats(generator);
		float scale = float(i) / 16.0;

		// scale samples s.t. they're more aligned to center of kernel
		scale = Lerp(0.1f, 1.0f, scale * scale);
		sample *= scale;
		ssaoKernel.push_back(sample);
	}


	std::vector<fVector3> ssaoNoise;
	for (unsigned int i = 0; i < 16; i++)
	{
		fVector3 noise(randomFloats(generator) * 2.0 - 1.0, randomFloats(generator) * 2.0 - 1.0, 0.0f); // rotate around z-axis (in tangent space)
		ssaoNoise.push_back(noise);
	}


	_ssaoNoise = new Texture(4, 4, GL_RGB32F, GL_RGB, GL_FLOAT, false);
	_ssaoNoise->Bind();
	{
		_ssaoNoise->SetData(&ssaoNoise[0]);

		_ssaoNoise->SetParameter(GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		_ssaoNoise->SetParameter(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		_ssaoNoise->SetParameter(GL_TEXTURE_WRAP_S, GL_REPEAT);
		_ssaoNoise->SetParameter(GL_TEXTURE_WRAP_T, GL_REPEAT);
	}
	_ssaoNoise->Unbind();


	_ssaoKernelTexture = new Texture(8, 8, GL_RGB32F, GL_RGB, GL_FLOAT, false);
	_ssaoKernelTexture->Bind();
	{
		_ssaoKernelTexture->SetData(&ssaoKernel[0]);

		_ssaoKernelTexture->SetParameter(GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		_ssaoKernelTexture->SetParameter(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		_ssaoKernelTexture->SetParameter(GL_TEXTURE_WRAP_S, GL_REPEAT);
		_ssaoKernelTexture->SetParameter(GL_TEXTURE_WRAP_T, GL_REPEAT);
	}
	_ssaoKernelTexture->Unbind();




	_renderBatch = new RenderBatch(_cont);

	FLAME_INFO("Scene finished loading");
}

void Scene::Update()
{

	_cameraStack.top()->Update();

	for (auto i = actorCollection.begin(); i != actorCollection.end(); i++)
	{
		i->second->Update(NULL);
	}


	pPxScene->simulate(1.0f / 60.0f);
	pPxScene->fetchResults(true);


	pUxService->Update();

}
void Scene::Render()
{

	_frameBuffer->Bind();
	{

		glClearColor(0.0f, 1.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		PushCamera(LightCollection[0].LightCamera());


		for (auto i = actorCollection.begin(); i != actorCollection.end(); i++)
		{
			i->second->Render();
		}


		PopCamera();

	}
	_frameBuffer->Unbind();


	_blurFrameBuffer->Bind();
	{

		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		gaussBlur->UseProgram();

		gaussBlur->SetVector("Direction", fVector2(0.0f, 1.0f));

		_renderBatch->DrawTexture(_depthMap, 0, 0, 2560.0f, 1440.0f, gaussBlur);
	}
	_blurFrameBuffer->Unbind();

	_frameBuffer->Bind();
	{

		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		gaussBlur->UseProgram();

		gaussBlur->SetVector("Direction", fVector2(1.0f, 0.0f));

		_renderBatch->DrawTexture(_blurTexture, 0, 0, 2560.0f, 1440.0f, gaussBlur);
	}
	_frameBuffer->Unbind();
	

	_sceneFrameBuffer->Bind();
	{



		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);



		for (auto i = actorCollection.begin(); i != actorCollection.end(); i++)
		{
			i->second->Render();
		}


	}
	_sceneFrameBuffer->Unbind();


	// SSAO SHADER -----------------------------------------------

	_ssaoFrameBuffer->Bind();
	{
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


		_ssaoShader->UseProgram();
		_ssaoShader->SetMatrix("Projection",		CurrentCamera()->Projection);
		_ssaoShader->SetMatrix("InverseProjection",	CurrentCamera()->ProjectionInverse);
		_ssaoShader->SetMatrix("SceneView",			CurrentCamera()->View);
		_ssaoShader->SetInt("KernelSize",			24);

		int pLocation = glGetUniformLocation(_ssaoShader->_programID, "gDepth");
		int nLocation = glGetUniformLocation(_ssaoShader->_programID, "gNormal");
		int tnLocation = glGetUniformLocation(_ssaoShader->_programID, "texNoise");

		glUniform1i(pLocation, 0);
		glUniform1i(nLocation, 1);
		glUniform1i(tnLocation, 2);
		
		_renderBatch->DrawTextures(3, new Texture * [3]{ _gBuffer, _nBuffer, _ssaoNoise }, 0, 0, 2560, 1440, _ssaoShader);

	}
	_ssaoFrameBuffer->Unbind();
	
	_blurFrameBuffer->Bind();
	{

		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		gaussBlur->UseProgram();

		gaussBlur->SetVector("Direction", fVector2(0.0f, 1.0f));

		_renderBatch->DrawTexture(_ssaoTexture, 0, 0, 2560.0f, 1440.0f, gaussBlur);
	}
	_blurFrameBuffer->Unbind();

	_ssaoFrameBuffer->Bind();
	{

		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		gaussBlur->UseProgram();

		gaussBlur->SetVector("Direction", fVector2(1.0f, 0.0f));

		_renderBatch->DrawTexture(_blurTexture, 0, 0, 2560.0f, 1440.0f, gaussBlur);
	}
	_ssaoFrameBuffer->Unbind();



	// SSAO SHADER -----------------------------------------------


	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);



	_ssaoFinal->UseProgram();
	_ssaoFinal->SetMatrix("InverseProjection",		CurrentCamera()->ProjectionInverse);
	_ssaoFinal->SetMatrix("InverseView",			CurrentCamera()->ViewInverse);
	_ssaoFinal->SetMatrix("LightViewProjection",	LightCollection[0].Projection * LightCollection[0].View);

	_ssaoFinal->SetVector("DirectionalLights[0].Direction",		LightCollection[0].Direction);
	_ssaoFinal->SetVector("DirectionalLights[0].Color",			LightCollection[0].LightColor);
	_ssaoFinal->SetFloat("DirectionalLights[0].Intensity",		LightCollection[0].Intensity);


	int pLoc = glGetUniformLocation(_ssaoFinal->_programID, "gDepth");
	int nLoc = glGetUniformLocation(_ssaoFinal->_programID, "gNormal");
	int aLoc = glGetUniformLocation(_ssaoFinal->_programID, "gAlbedo");
	int ssLoc = glGetUniformLocation(_ssaoFinal->_programID, "ssao");
	int smLoc = glGetUniformLocation(_ssaoFinal->_programID, "_shadowMap");

	glUniform1i(pLoc, 0);
	glUniform1i(nLoc, 1);
	glUniform1i(aLoc, 2);
	glUniform1i(ssLoc, 3);
	glUniform1i(smLoc, 4);


	_renderBatch->DrawTextures(5, new Texture * [5]{ _gBuffer, _nBuffer, _aBuffer, _ssaoTexture, _depthMap }, 0, 0, 2560, 1440, _ssaoFinal);





	pUxService->Render();

}
void Scene::AddActor(std::string _id, Actor* _ac)
{
	if (actorCollection.find(_id) == actorCollection.end())
	{

		_ac->_scene = this;

		actorCollection[_id] = _ac;

		if (_ac->pPxActor != NULL)
		{
			pPxScene->addActor(*_ac->pPxActor);

			//printf("%s has pxActor with adress: %p\n", _id.c_str(), _ac->pPxActor);

		}
	}

}
void Scene::RemoveActor(std::string _id)
{
	actorCollection.erase(_id);

}

void Scene::PushCamera(Camera* _cam)
{
	if (_cam != NULL)
	{
		_cameraStack.push(_cam);
	}
}


Camera* Scene::PopCamera()
{
	if (!_cameraStack.size())
		return NULL;

	Camera* _cameraPtr = _cameraStack.top();

	_cameraStack.pop();

	if (CurrentCamera() == NULL)
	{
		PopCamera();
	}

	return _cameraPtr;
}

inline Camera* Scene::CurrentCamera()
{
	return _cameraStack.top();
}

