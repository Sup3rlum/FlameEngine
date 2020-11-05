#include "Scene.h"



Scene::Scene(Context* _cont)
{
	_context = _cont;



	_camera = new Camera(_cont, CameraType::FIRSTPERSON, ProjectionType::PERSPECTIVE);

	gaussBlur = Shader::FromSource(".\\shaders\\renderbatch.vert", ".\\shaders\\gauss_blur.frag");

	_tex = new Texture(".\\textures\\dirt.jpg");

	_depthMap = new Texture(2560, 1440, GL_RG32F, GL_RGBA, GL_FLOAT, true);
	_depthMap->Bind();
	_depthMap->SetParameter(GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	_depthMap->SetParameter(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	_depthMap->SetParameter(GL_TEXTURE_WRAP_S, GL_REPEAT);
	_depthMap->SetParameter(GL_TEXTURE_WRAP_T, GL_REPEAT);
	_depthMap->SetParameterf(GL_TEXTURE_MAX_ANISOTROPY_EXT, 16);
	_depthMap->Unbind();


	_depth = new Texture(2560, 1440, GL_DEPTH_COMPONENT, GL_DEPTH_COMPONENT, GL_FLOAT, true);
	_depth->Bind();
	_depth->SetParameter(GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	_depth->SetParameter(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	_depth->SetParameter(GL_TEXTURE_WRAP_S, GL_REPEAT);
	_depth->SetParameter(GL_TEXTURE_WRAP_T, GL_REPEAT);
	_depth->Unbind();


	_blurTexture = new Texture(2560, 1440, GL_RG32F, GL_RGBA, GL_FLOAT, true);
	_blurTexture->Bind();
	_blurTexture->SetParameter(GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	_blurTexture->SetParameter(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	_blurTexture->SetParameter(GL_TEXTURE_WRAP_S, GL_REPEAT);
	_blurTexture->SetParameter(GL_TEXTURE_WRAP_T, GL_REPEAT);
	_blurTexture->Unbind();


	_frameBuffer = new FrameBuffer(2560, 1440);
	_frameBuffer->Bind();
	_frameBuffer->BindTexture(_depthMap, GL_COLOR_ATTACHMENT0);
	_frameBuffer->BindTexture(_depth, GL_DEPTH_ATTACHMENT);
	_frameBuffer->Unbind();

	_blurFrameBuffer = new FrameBuffer(2560, 1440);
	_blurFrameBuffer->Bind();
	_blurFrameBuffer->BindTexture(_blurTexture, GL_COLOR_ATTACHMENT0);
	_blurFrameBuffer->Unbind();


	_depthRenderState = new RenderState();

	_depthRenderState->CullState = CullState::Back;
	_depthRenderState->DepthFunction = DepthFunc::LessOrEqual;

	_renderBatch = new RenderBatch(_cont);
	DebugView::Init(_cont);
}

void Scene::Update()
{
	_camera->Update();

	DebugView::Update();
}
void Scene::Render()
{

	_frameBuffer->Bind();
	{

		glClearColor(0.6f, 0.6f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		for (auto i = actorCollection.begin(); i != actorCollection.end(); i++)
		{
			i->second->RenderDepth();
		}


	}
	_frameBuffer->Unbind();


	_blurFrameBuffer->Bind();
	{

		glClearColor(0.6f, 0.6f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		gaussBlur->UseProgram();

		gaussBlur->SetVector("Direction", fVector2(0.0f, 1.0f));

		_renderBatch->DrawTexture(_depthMap, 0, 0, 2560.0f, 1440.0f, gaussBlur);
	}
	_blurFrameBuffer->Unbind();

	_frameBuffer->Bind();
	{

		glClearColor(0.6f, 0.6f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		gaussBlur->UseProgram();

		gaussBlur->SetVector("Direction", fVector2(1.0f, 0.0f));

		_renderBatch->DrawTexture(_blurTexture, 0, 0, 2560.0f, 1440.0f, gaussBlur);
	}
	_frameBuffer->Unbind();




	/*_frameBuffer->Bind();
	{
		glClearColor(0.6f, 0.6f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		gaussBlur->UseProgram();

		gaussBlur->SetVector("Direction", fVector2(0.0f, 1.0f));
		gaussBlur->SetVector("Resolution", fVector2(2560.0f, 1440.0f));

		_renderBatch->DrawTexture(_blurTexture, 0, 0, 2560, 1440, gaussBlur);
	}
	_frameBuffer->Unbind();*/

	glClearColor(0.0f, 0.6f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	for (auto i = actorCollection.begin(); i != actorCollection.end(); i++)
	{
		i->second->Render();
	}

	//_renderBatch->DrawTexture(_tex, 600.0f, 600.0f, 400.0f, 400.0f);
	//_renderBatch->DrawTexture(_depthMap, 600.0f, 0.0f, 400.0f, 400.0f);
	//_renderBatch->DrawTexture(_depth, 1000.0f, 0.0f, 400.0f, 400.0f);
	//_renderBatch->DrawTexture(_blurTexture, 1400.0f, 0.0f, 400.0f, 400.0f);

	DebugView::Draw(_camera);

}
void Scene::AddActor(std::string _id, Actor* _ac)
{
	if (actorCollection.find(_id) == actorCollection.end())
	{

		_ac->_scene = this;

		actorCollection[_id] = _ac;
	}

}
void Scene::RemoveActor(std::string _id)
{
	actorCollection.erase(_id);

}
