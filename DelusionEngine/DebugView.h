#pragma once

#include "nchfx.h"
#include "BoundingBox.h"
#include "Plane.h"
#include "Camera.h"
#include "VertexBuffer.h"
#include "Vertex.h"
#include "Shader.h"
#include "RenderBatch.h"

using namespace std;
using namespace glm;

class DELUSION_DLL DebugView
{
public:
	static vector<Plane*> _planes;
	static vector<BoundingBox*> _boxes;
	static void Draw(Camera* _cam);
	static void Update(FrameTime* _fr);

	static void Init(ContextDescription* _cont);

	static VertexBuffer* _vb;
	static Shader* _shader;
	static RenderBatch* _renderBatch;
	static Font* _debugFont;

	static int _fps;
};

