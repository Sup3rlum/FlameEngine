
#include "Core/Engine/FlameRI/FRI.h"
#include "OpenGL/GLHBAOPlus.h"
#include "DX11/D3D11HBAOPlus.h"

HBAOPlus* HBAOPlus::Allocate(FRIContext* renderContext, FHBAOParameters params)
{
	switch (renderContext->InstanceDescription.RenderFramework)
	{
		case EFRIRendererFramework::OpenGL:
		{
			return new GLHBAOPlus(params);
		}
		case EFRIRendererFramework::DX11:
		{
			D3D11FRIContext* d3dContext = static_cast<D3D11FRIContext*>(renderContext);

			return new D3D11HBAOPlus(params, d3dContext->Device, d3dContext->DeviceContext);
		}
		default:
			return nullptr;

	}
}