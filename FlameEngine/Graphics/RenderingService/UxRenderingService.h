#pragma once


#include "../../dll/nchfx.h"
#include "../Common/Texture.h"
#include "../ShaderDefinitions/Shader.h"
#include "../Common/VertexBuffer.h"
#include "../Common/Vertex.h"
#include "../UserInterface/UxFont.h"
#include "../Common/RenderState.h"
#include "../../Context/ContextDescription.h"
#include "../../Context/Context.h"


#include "../UserInterface/UxRectangle.h"

EXPORT(class,  UxRenderingService)
{
public:
		UxRenderingService(Context* _context);

		void UseShader();
		void DrawElement();

		void SetView();
		void SetView(fMatrix4 view);


		Shader* mDefaultComponentShader;
		VertexBuffer* mVertexBuffer;
		fMatrix4 mView;
		Context* mAttachedContext;
};
