#pragma once


#include "GameObjects/FpsCamera.h"
#include "GameObjects/Actor.h"
#include "GameObjects/Lighting/DirectionalLight.h"
#include "Environment/Sky.h"
#include "DebugView.h"
#include "../Graphics/Shader.h"
#include "../Graphics/RenderBatch.h"
#include "../Graphics/FrameBuffer.h"
#include "../Context/Context.h"
#include "../IO/AssetManager.h"
#include "../Mathematics/Module.h"

EXPORT_CLASS Scene
{
public:
	Scene(Context* _context);
	~Scene();

	void Update();
	void Render();

	stack<Camera*> _cameraStack;

	void AddActor(STRING id, Actor* ac);
	void RemoveActor(STRING id);

	void PushCamera(Camera* _cam);
	Camera* PopCamera();
	Camera* CurrentCamera();

	std::vector<DirectionalLight> LightCollection;

	FrameBuffer* _frameBuffer;
	FrameBuffer* _blurFrameBuffer;
	Context* _context;

	RenderBatch* _renderBatch;

	RenderState* _depthRenderState;
	RenderState* _SkyRenderState;

	Shader* gaussBlur;
	Shader* _ssaoShader;
	Shader* _ssaoFinal;

	Texture* _tex;

	Texture* _blurTexture;

	FrameBuffer* _sceneFrameBuffer;
	FrameBuffer* _ssaoFrameBuffer;

	Texture* _gBuffer;
	Texture* _nBuffer;
	Texture* _aBuffer;
	Texture* _depthMap;


	Texture* _ssaoKernelTexture;
	Texture* _ssaoTexture;

	Texture* _ssaoNoise;

	Sky* _skybox;

	std::vector<fVector3> ssaoKernel;

private:
	float lerp(float a, float b, float c);
	std::unordered_map<STRING, Actor*> actorCollection;
};

