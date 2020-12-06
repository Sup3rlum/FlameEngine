#pragma once


#include "../ShaderDefinitions/Program.h"
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
	FMatrix4 View;
	RenderState* mRenderState;
	Context* mAttachedContext;
};

