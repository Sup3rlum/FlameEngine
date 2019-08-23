#pragma once

#include "nchfx.h"
#include "BoundingBox.h"
#include "Plane.h"
#include "Camera.h"
#include "VertexBuffer.h"
#include "Vertex.h"
#include "Shader.h"

using namespace std;
using namespace glm;

class DELUSION_DLL DebugView
{
public:
	static vector<Plane*> _planes;
	static vector<BoundingBox*> _boxes;
	static void Draw(Camera* _cam);

	static void Init();

	static VertexBuffer* _vb;
	static Shader* _shader;
};

