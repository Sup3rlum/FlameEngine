#include "UxContainer.h"
#include "UxComponent.h"

/*
UxContainer::UxContainer(Context* context)
{
	pRenderingService = new UxRenderingService(context);
}


void UxContainer::AddComponent(UxComponent* c)
{
	if (c != NULL)
	{
		c->SetParent(this);
		mDataInternal.push_back(c);
	}
}


void UxContainer::RenderComponents()
{
	for (int i = 0; i < mDataInternal.size(); i++)
	{
		if (mDataInternal[i]->mVisible)
			mDataInternal[i]->Render();
	}
}

void UxContainer::UpdateComponents()
{
	for (int i = 0; i < mDataInternal.size(); i++)
	{
		mDataInternal[i]->Update();
	}
}

FVector2 UxContainer::GetCursorPosition()
{
	double x, y;

	pRenderingService->mAttachedContext->GetCursorPosition(&x, &y);

	return FVector2(x, y);
}*/