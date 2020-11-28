#pragma once


#include "../../Graphics/ShaderDefinitions/Shader.h"
#include "../../Graphics/FrameBuffer.h"
#include "../../Context/Context.h"
#include "../../Graphics/VertexBuffer.h"
#include "../../Graphics/Vertex.h"
#include "../../Graphics//RenderState.h"

EXPORT_CLASS RenderingServiceBase
{
public:
	RenderingServiceBase(Context * context);

	VertexBuffer* mQuadBuffer;
	fMatrix4 View;
	RenderState* mRenderState;
	Context* mAttachedContext;
};

