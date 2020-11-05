#pragma once


#include "GameObjects/Camera.h"
#include "GameObjects/Actor.h"
#include "GameObjects/Lighting/DirectionalLight.h"
#include "DebugView.h"
#include "../Graphics/Shader.h"
#include "../Graphics/RenderBatch.h"
#include "../Graphics/FrameBuffer.h"
#include "../Context/Context.h"


EXPORT_CLASS Scene
{
public:
	Scene(Context* _context);
	~Scene();

	void Update();
	void Render();

	Camera* _camera;

	void AddActor(STRING id, Actor* ac);
	void RemoveActor(STRING id);

	std::vector<DirectionalLight> LightCollection;

	Texture* _depthMap;
	FrameBuffer* _frameBuffer;
	FrameBuffer* _blurFrameBuffer;
	Context* _context;

	RenderBatch* _renderBatch;

	RenderState* _depthRenderState;

	Shader* gaussBlur;

	Texture* _tex;
	Texture* _depth;

	Texture* _blurTexture;

private:
	
	std::unordered_map<STRING, Actor*> actorCollection;
};

