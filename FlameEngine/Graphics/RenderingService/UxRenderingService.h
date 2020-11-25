#pragma once


#include "../../dll/nchfx.h"
#include "../../Graphics/Texture.h"
#include "../../Graphics/ShaderDefinitions/Shader.h"
#include "../../Graphics/VertexBuffer.h"
#include "../../Graphics/Vertex.h"
#include "../../Graphics/UserInterface/UxFont.h"
#include "../../Graphics/RenderState.h"
#include "../../Context/ContextDescription.h"
#include "../../Context/Context.h"


#include "../UserInterface/UxRectangle.h"

EXPORT_CLASS UxRenderingService
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

