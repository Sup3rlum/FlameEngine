#pragma once

#include "FRIResource.h"


/*
* 
*	I have defined some hacky-ish macros to help code commands faster and help sake readability.
* 
*	FRegiterCommand creates a struct with set cmdName prefixed with FRICommand
*	e.g. FRegisterCommand(CallMom) -> struct FRICommandCallMom : FRICommand<FRICommandCallMom>
* 
*	FRegisterCommandMultiTemplate does the same job, but allows for template specialization, slotting the generic
*	parameterizations in their proper places upon declaration
*	e.g. FRegisterCommand(CallDad, TGenType1, TGenType2, TGenType3) ->
*	
*	template<typename TGenType1, typename TGenType2, typename TGenType3>
*	struct FRICommandCallDad : FRICommand<FRICommandCallDad<TGenType1, TGenType2,TGenType3>>
* 
*	FRICmdInit initializes a __forceinline constructor with the prefixed name matching the struct definition name
* 
*/

#define FTEMPLATETYPENAME(genType) typename genType


#define _GET_NTH_ARG(_1, _2, _3, _4, _5, N, ...) N


#define _fe_0(_call, ...)
#define _fe_1(_call, x) _call(x)
#define _fe_2(_call, x, ...) _call(x) _fe_1(_call, __VA_ARGS__)
#define _fe_3(_call, x, ...) _call(x) _fe_2(_call, __VA_ARGS__)
#define _fe_4(_call, x, ...) _call(x) _fe_3(_call, __VA_ARGS__)

#define CALL_MACRO_X_FOR_EACH(x, ...) \
    _GET_NTH_ARG("ignored", ##__VA_ARGS__, \
    _fe_4, _fe_3, _fe_2, _fe_1, _fe_0)(x, ##__VA_ARGS__)


#define FRegisterFRICommand(cmdName) struct FRICommand##cmdName : FRICommand<FRICommand##cmdName>

#define FRegisterFRICommandMultiTemplate(cmdName, ...) template<CALL_MACRO_X_FOR_EACH(FTEMPLATETYPENAME, __VA_ARGS__)> \
														struct FRICommand##cmdName : FRICommand<FRICommand##cmdName<__VA_ARGS__>>


#define FRICmdInit(name) FINLINE FRICommand##name

struct FRICommandBase
{
	FRICommandBase* Next = NULL;

	virtual void ExecuteCmd();
};


template<typename FCmdParam>
struct FRICommand : FRICommandBase
{
	FINLINE void ExecuteCmd()
	{
		FCmdParam* cmd = static_cast<FCmdParam*>(this);

		cmd->Execute();
		cmd->~FCmdParam();
	}

};


/*
* 
*	VIEWPORT
* 
* 
*/

FRegisterFRICommand(SetViewport)
{
	uint32 x;
	uint32 y;
	uint32 width;
	uint32 height;

	FRICmdInit(SetViewport)(uint32 x, uint32 y, uint32 width, uint32 height) :
		x(x),
		y(y),
		width(width),
		height(height)
	{

	}

	void Execute();
};

/*
* 
*  FRAMEBUFFER
* 
*/


FRegisterFRICommandMultiTemplate(BindFrameBuffer, GenFrameBufferPtr)
{
	GenFrameBufferPtr frameBuffer;

	FRICmdInit(BindFrameBuffer)(GenFrameBufferPtr frameBuffer) :
		frameBuffer(frameBuffer)
	{

	}

	void Execute();

};


FRegisterFRICommandMultiTemplate(ClearBufferColor, GenFrameBufferPtr)
{
	GenFrameBufferPtr frameBuffer;
	Color color;

	FRICmdInit(ClearBufferColor)(GenFrameBufferPtr frameBuffer, Color color) :
		frameBuffer(frameBuffer),
		color(color)
	{

	}

	void Execute();

};

FRegisterFRICommandMultiTemplate(ClearBuffer, GenFrameBufferPtr)
{
	GenFrameBufferPtr frameBuffer;
	uint32 clearbit;

	FRICmdInit(ClearBuffer)(GenFrameBufferPtr frameBuffer, uint32 clearbit) :
		frameBuffer(frameBuffer),
		clearbit(clearbit)
	{

	}

	void Execute();

};


FRegisterFRICommandMultiTemplate(CopyBufferData, GenFrameBufferPtr)
{
	GenFrameBufferPtr frameBufferSource;
	GenFrameBufferPtr frameBufferDest;
	uint32 clearbit;

	FRICmdInit(ClearBufferData)(GenFrameBufferPtr frameBufferSource, GenFrameBufferPtr frameBufferDest, uint32 clearbit) :
		frameBufferSource(frameBufferSource),
		frameBufferDest(frameBufferDest),
		clearbit(clearbit)
	{

	}

	void Execute();

};


FRegisterFRICommand(SetDepthCompareFunc)
{
	uint32 depthfunc;
	FRICmdInit(SetDepthCompareFunc)(uint32 depthfunc) :
		depthfunc(depthfunc)
	{

	}
	void Execute();
};



FRegisterFRICommand(SetAlphaBlendFunc)
{
	uint32 blendfunc;
	FRICmdInit(SetAlphaBlendFunc)(uint32 blendfunc) :
		blendfunc(blendfunc)
	{

	}
	void Execute();
};



FRegisterFRICommand(SetPrimitiveCullMode)
{
	uint32 cullmode;
	FRICmdInit(SetPrimitiveCullMode)(uint32 cullmode) :
		cullmode(cullmode)
	{

	}
	void Execute();
};




/*
* 
*  BUFFER RENDERING
* 
*/


FRegisterFRICommand(SetGeometrySource)
{
	FResourceVertexBuffer* vertexBuffer;

	FRICmdInit(SetGeometrySource)(FResourceVertexBuffer * vertexBuffer) :
		vertexBuffer(vertexBuffer)
	{

	}
	void Execute();

};



FRegisterFRICommand(DrawPrimitives)
{
	uint32 elementType;
	uint32 elementCount;

	FRICmdInit(DrawPrimitives)(uint32 elementType, uint32 elementCount) :
		elementType(elementType),
		elementCount(elementCount)
	{
		
	}

	void Execute();

};

FRegisterFRICommand(DrawPrimitivesIndexed)
{

	uint32 elementType;
	uint32 elementCount;
	uint32 indexType;
	FResourceIndexBuffer* indexBuffer;

	FRICmdInit(DrawPrimitivesIndexed)(uint32 elementType, uint32 elementCount, uint32 indexType, FResourceIndexBuffer* indexBuffer) :
		elementType(elementType),
		elementCount(elementCount),
		indexType(indexType),
		indexBuffer(indexBuffer)
	{

	}

	void Execute();

};


/*
* 
* SHADERS
* 
*/



FRegisterFRICommandMultiTemplate(SetShader, GenResourceShader)
{
	GenResourceShader* shader;

	FRICmdInit(SetShader)(GenResourceShader * shader) :
		shader(shader)
	{

	}

	void Execute();
};


FRegisterFRICommandMultiTemplate(SetShaderUniformBuffer, GenResourceShader)
{
	GenResourceShader* shader;
	FResourceUniformBuffer* uniformBuffer;

	FRICmdInit(SetShader)(GenResourceShader * shader, FResourceUniformBuffer uniformBuffer) :
		shader(shader),
		uniformBuffer(uniformBuffer)
	{

	}

	void Execute();
};


/*
* 
* TEXTURES
* 
*/


FRegisterFRICommand(SetTextureStorage2D)
{
	uint32 innerColorFormat;
	uint32 coverColorFormat;
	uint32 dataColorFormat;

	FRICmdInit(SetTextureStorage2D)(uint32 innerColorFormat, uint32 coverColorFormat, uint32 dataColorFormat) :
		innerColorFormat(innerColorFormat),
		coverColorFormat(coverColorFormat),
		dataColorFormat(dataColorFormat)
	{

	}

	void Execute();
};


FRegisterFRICommand(SetTextureStorage3D)
{
	uint32 innerColorFormat;
	uint32 coverColorFormat;
	uint32 dataColorFormat;

	FRICmdInit(SetTextureStorage3D)(uint32 innerColorFormat, uint32 coverColorFormat, uint32 dataColorFormat):
		innerColorFormat(innerColorFormat),
		coverColorFormat(coverColorFormat),
		dataColorFormat(dataColorFormat)
	{

	}

	void Execute();
};


FRegisterFRICommandMultiTemplate(SetTextureParameterBuffer, GenResourceTexture)
{
	GenResourceTexture* texture;
	FResourceTextureParameterBuffer* parameterBuffer;

	FRICmdInit(SetTextureParameterBuffer)(GenResourceTexture * texture, FResourceTextureParameterBuffer parameterBuffer) :
		texture(texture),
		parameterBuffer(parameterBuffer)
	{

	}

	void Execute();
};



/*
* 
* 
* COMPUTE
* 
* 
*/





/*
* 
* 
* MISC
* 
* 
*/


FRegisterFRICommandMultiTemplate(BeginOcclusionQuery, GenOcclusionQueryPtr)
{
	GenOcclusionQueryPtr occlusionQuery;

	FRICmdInit(BeginOcclusionQuery)(GenOcclusionQueryPtr occlusionQuery) :
		occlusionQuery(occlusionQuery)
	{

	}

	void Execute();
};


FRegisterFRICommandMultiTemplate(EndOcclusionQuery, GenOcclusionQueryPtr)
{
	GenOcclusionQueryPtr occlusionQuery;

	FRICmdInit(EndOcclusionQuery)(GenOcclusionQueryPtr occlusionQuery) :
		occlusionQuery(occlusionQuery)
	{

	}

	void Execute();
};