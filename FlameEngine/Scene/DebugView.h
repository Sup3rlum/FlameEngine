#pragma once

#include "../dll/nchfx.h"
#include "BoundingBox.h"
#include "Plane.h"
#include "Camera.h"
#include "../Graphics/VertexBuffer.h"
#include "../Graphics/Vertex.h"
#include "../Graphics/Shader.h"
#include "../Graphics/RenderBatch.h"

using namespace std;
using namespace glm;

EXPORT_CLASS DebugView
{
public:
	static vector<Plane*> _planes;
	static vector<BoundingBox*> _boxes;
	static void Draw(Camera* _cam);
	static void Update();

	static void Init(Context* _context);

	static VertexBuffer* _vb;
	static Shader* _shader;
	static RenderBatch* _renderBatch;
	static Font* _debugFont;

	static int _fps;
	static char _gpuName[256];
};

