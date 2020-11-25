#pragma once

#include "../../dll/nchfx.h"
#include "../../Context/Context.h"
#include "../../Graphics/Common/RenderBatch.h"
#include "../RenderingService/UxRenderingService.h"


EXPORT_CLASS UxComponent;


EXPORT_CLASS UxContainer
{
public:

	UxContainer(Context* context);

	void AddComponent(UxComponent* component);

	void RenderComponents();
	void UpdateComponents();

	fVector2 GetCursorPosition();


	std::vector<UxComponent*> mDataInternal;

	UxRenderingService* pRenderingService;
};

