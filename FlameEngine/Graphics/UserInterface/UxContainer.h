#pragma once

#include "../../dll/nchfx.h"
#include "../../Context/Context.h"
#include "../../Graphics/Common/RenderBatch.h"
#include "../RenderingService/UxRenderingService.h"


EXPORT_FORWARD_DECL(class,  UxComponent);


EXPORT(class,  UxContainer)
{
public:

	UxContainer(Context* context);

	void AddComponent(UxComponent* component);

	void RenderComponents();
	void UpdateComponents();

	FVector2 GetCursorPosition();


	std::vector<UxComponent*> mDataInternal;

	UxRenderingService* pRenderingService;
};

