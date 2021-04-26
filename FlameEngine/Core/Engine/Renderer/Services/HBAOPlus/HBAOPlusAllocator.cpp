
#include "Core/Engine/FlameRI/FRI.h"
#include "OpenGL/GLHBAOPlus.h"

HBAOPlus* HBAOPlus::Allocate(FRIContext* renderContext, FHBAOParameters params)
{
	switch (renderContext->RenderFramework)
	{
		case EFRIRendererFramework::OpenGL:
			return new GLHBAOPlus(params);
		default:
			return nullptr;

	}
}