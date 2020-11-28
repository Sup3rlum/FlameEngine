#include "RenderingServiceBase.h"


RenderingServiceBase::RenderingServiceBase(Context* context)
{
	mAttachedContext = context;

	View = fMatrix4::CreateOrthographic(0.0f, (float)context->_contextDescription->width, (float)context->_contextDescription->height, 0.0f, 0.0f, 1.0f);



	VertexTexture mQuadData[4] =
	{
		VertexTexture(fVector3(0,0,0), fVector2(0,1)),
		VertexTexture(fVector3(1,0,0), fVector2(1,1)),
		VertexTexture(fVector3(1,1,0), fVector2(1,0)),
		VertexTexture(fVector3(0,1,0), fVector2(0,0))
	};
	GLuint mQuadIndexData[6]
	{
		0,
		1,
		2,

		0,
		2,
		3
	};



	mQuadBuffer = new VertexBuffer(VertexTexture::Elements);
	mQuadBuffer->SetIndexedData<VertexTexture>(mQuadData, mQuadIndexData, 4, 6);

	mRenderState = new RenderState();
	mRenderState->CullState = CullState::Front;
	mRenderState->DepthFunction = DepthFunc::Always;
	mRenderState->SourceBlend = BlendFunc::SourceAlpha;
	mRenderState->DestinationBlend = BlendFunc::OneMinusSourceAlpha;

}
