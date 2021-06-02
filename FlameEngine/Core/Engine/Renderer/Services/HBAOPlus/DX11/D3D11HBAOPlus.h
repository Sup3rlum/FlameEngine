#pragma once
#include "../HBAOPlusInterface.h"

#include "Core/Engine/FlameRI/DX11/D3D11FRI.h"
#include <GFSDK_SSAO.h>


class D3D11HBAOPlus : public HBAOPlus
{
public:

	D3D11HBAOPlus(FHBAOParameters params, ID3D11Device* d3dDevice, ID3D11DeviceContext* d3dDeviceContext);
	void RenderAO(FRITexture2D* depthTex, FRITexture2D* normTex, FRIFrameBuffer* outputFBO, const FMatrix4& projection, const FMatrix4& view);

	GFSDK_SSAO_Parameters	mAoParams;
	GFSDK_SSAO_Context_D3D11* mHbaoD3D11Context;

	ID3D11Device* d3dDevice;
	ID3D11DeviceContext* d3dDeviceContext;
};