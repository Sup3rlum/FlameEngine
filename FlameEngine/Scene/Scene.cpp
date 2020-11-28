#include "Scene.h"


GLenum attachments[4] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 };




Scene::Scene(Context* _cont)
{
	_context = _cont;


	pPhysXService = new PhysXService();


	pUxService = new UxService();

	pUxContainer = new UxContainer(_cont);
	pUxService->PushContainer(pUxContainer);

	cons = new UxConsoleWindow();
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

	_depthMap = new Texture(2560, 1440, GL_RGBA32F, GL_RGBA, GL_FLOAT, true);
	_depthMap->SetFilteringMode(TextureFiltering::BILINEAR);
	_depthMap->SetWrappingMode(TextureWrapping::REPEAT);


	_blurTexture = new Texture(2560, 1440, GL_RGBA32F, GL_RGBA, GL_FLOAT, true);
	_blurTexture->SetFilteringMode(TextureFiltering::BILINEAR);
	_blurTexture->SetWrappingMode(TextureWrapping::REPEAT);



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


	_gBuffer = new Texture(2560, 1440, GL_RGBA32F, GL_RGBA, GL_FLOAT, false);
	_gBuffer->SetFilteringMode(TextureFiltering::BILINEAR);
	_gBuffer->SetWrappingMode(TextureWrapping::REPEAT);

	_nBuffer = new Texture(2560, 1440, GL_RGBA32F, GL_RGBA, GL_FLOAT, false);
	_nBuffer->SetFilteringMode(TextureFiltering::BILINEAR);
	_nBuffer->SetWrappingMode(TextureWrapping::REPEAT);


	_aBuffer = new Texture(2560, 1440, GL_RGBA32F, GL_RGBA, GL_FLOAT, false);
	_aBuffer->SetFilteringMode(TextureFiltering::BILINEAR);
	_aBuffer->SetWrappingMode(TextureWrapping::REPEAT);



	_ssaoTexture = new Texture(2560, 1440, GL_RGBA32F, GL_RGBA, GL_FLOAT, false);
	_ssaoTexture->SetFilteringMode(TextureFiltering::BILINEAR);
	_ssaoTexture->SetWrappingMode(TextureWrapping::REPEAT);


	msGBuffer = new MultisampleTexture(2560, 1440, GL_RGBA32F, _cont->_contextDescription->contextSampleCount);
	msGBuffer->SetFilteringMode(TextureFiltering::BILINEAR);
	msGBuffer->SetWrappingMode(TextureWrapping::REPEAT);


	_sceneFrameBuffer = new FrameBuffer(2560, 1440);
	_sceneFrameBuffer->Bind();
	_sceneFrameBuffer->BindTexture(_gBuffer, GL_COLOR_ATTACHMENT0);
	//_sceneFrameBuffer->BindTexture(msGBuffer, GL_COLOR_ATTACHMENT0);

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
	_ssaoNoise->SetData(&ssaoNoise[0]);
	_ssaoNoise->SetFilteringMode(TextureFiltering::BILINEAR);
	_ssaoNoise->SetWrappingMode(TextureWrapping::REPEAT);




	_ssaoKernelTexture = new Texture(8, 8, GL_RGB32F, GL_RGB, GL_FLOAT, false);
	_ssaoKernelTexture->SetData(&ssaoKernel[0]);
	_ssaoKernelTexture->SetFilteringMode(TextureFiltering::BILINEAR);
	_ssaoKernelTexture->SetWrappingMode(TextureWrapping::REPEAT);



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
	for (auto i = actorCollection.begin(); i != actorCollection.end(); i++)
	{
		i->second->Render();
	}

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

