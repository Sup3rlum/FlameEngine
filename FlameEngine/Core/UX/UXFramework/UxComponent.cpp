#include "UxComponent.h"


UxComponent::UxComponent() : mParent(NULL)
{
	mRenderState = new RenderState();

	mRenderState->CullState = CullState::Front;
	mRenderState->DepthFunction = DepthFunc::Always;

	mRenderState->SourceBlend = BlendFunc::SourceAlpha;
	mRenderState->DestinationBlend = BlendFunc::OneMinusSourceAlpha;

	mVisible = true;
}


UxContainer* UxComponent::GetParent()
{
	return mParent;
}
void UxComponent::SetParent(UxContainer* p)
{
	mParent = p;
}

void UxComponent::SetVisibility(bool visible)
{
	mVisible = visible;
}
bool UxComponent::GetVisibility()
{
	return	mVisible;
}



void UxComponent::Render()
{

}

void UxComponent::Update()
{

}