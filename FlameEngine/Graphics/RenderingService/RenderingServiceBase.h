#pragma once


#include "../ShaderDefinitions/Shader.h"
#include "../Common/FrameBuffer.h"
#include "../../Context/Context.h"
#include "../Common/VertexBuffer.h"
#include "../Common/Vertex.h"
#include "../Common/RenderState.h"

EXPORT(class,  RenderingServiceBase)
{
public:
	RenderingServiceBase(Context * context);

	VertexBuffer* mQuadBuffer;
	fMatrix4 View;
	RenderState* mRenderState;
	Context* mAttachedContext;
};
