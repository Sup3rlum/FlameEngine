#include "D3D11FRIContext.h"
#include "D3D11FRIDynamic.h"


LRESULT CALLBACK D3D11FRIContext::Win32MessageHandler(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam)
{
	switch (umessage)
	{
	case  WM_KEYDOWN:
	{
		uint32 repeatCount = lparam & 0x7FFF;

		if (repeatCount > 1)
		{
			InputHandlerDelegate((FKeyboardKeys)wparam, FKeyEvent::OnHold);
		}
		else
		{
			InputHandlerDelegate((FKeyboardKeys)wparam, FKeyEvent::OnPress);
		}
		break;
	}
	case WM_KEYUP:
	{
		InputHandlerDelegate((FKeyboardKeys)wparam, FKeyEvent::OnRelease);
		break;
	}

    case WM_MBUTTONDOWN:    InputHandlerDelegate2(FMouseButton::Middle, FKeyEvent::OnPress);    break;
    case WM_MBUTTONUP:      InputHandlerDelegate2(FMouseButton::Middle, FKeyEvent::OnRelease);  break;
    case WM_LBUTTONDOWN:    InputHandlerDelegate2(FMouseButton::Left,   FKeyEvent::OnPress);    break;
    case WM_LBUTTONUP:      InputHandlerDelegate2(FMouseButton::Left,   FKeyEvent::OnRelease);  break;
    case WM_RBUTTONDOWN:    InputHandlerDelegate2(FMouseButton::Right, FKeyEvent::OnPress);    break;
    case WM_RBUTTONUP:      InputHandlerDelegate2(FMouseButton::Right, FKeyEvent::OnRelease);  break;

  
	default:

		return DefWindowProc(hwnd, umessage, wparam, lparam);
	}
	return 0;
}


void D3D11FRIContext::Initialize()
{
    int PosX = 0, PosY = 0;

    if (InstanceDescription.IsFullscreen)
    {
        int screenWidth = GetSystemMetrics(SM_CXSCREEN);
        int screenHeight = GetSystemMetrics(SM_CYSCREEN);

        PosX = (screenWidth - InstanceDescription.Width) / 2;
        PosY = (screenHeight - InstanceDescription.Height) / 2;
    }

    if (win32Context == NULL)
    {
        win32Context = new Win32Context("Engine2", PosX, PosY, InstanceDescription.Width, InstanceDescription.Height, InstanceDescription.IsFullscreen, FWin32MessageProcDelegate::Make<D3D11FRIContext, &D3D11FRIContext::Win32MessageHandler>(this));
    }
    isActive = true;

    if (!InitializeDX())
    {
        return;
    }

    win32Context->Show();
}
bool D3D11FRIContext::InitializeDX()
{
	RECT clientRect;
	GetClientRect(win32Context->hWindow, &clientRect);

	uint32 clientWidth = clientRect.right - clientRect.left;
	uint32 clientHeight = clientRect.bottom - clientRect.top;

	DXGI_SWAP_CHAIN_DESC swapChainDesc;
	ZeroMemory(&swapChainDesc, sizeof(DXGI_SWAP_CHAIN_DESC));

	swapChainDesc.BufferCount = 1;
	swapChainDesc.BufferDesc.Width = clientWidth;
	swapChainDesc.BufferDesc.Height = clientHeight;
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    swapChainDesc.BufferDesc.RefreshRate = QueryRefreshRate(clientWidth, clientHeight, false);
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.OutputWindow = win32Context->hWindow;
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.SampleDesc.Quality = 0;
    //swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
    swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	swapChainDesc.Windowed = !InstanceDescription.IsFullscreen;


    UINT createDeviceFlags = 0;// D3D11_CREATE_DEVICE_DISABLE_GPU_TIMEOUT;
#if _DEBUG
	//createDeviceFlags = D3D11_CREATE_DEVICE_DEBUG;
#endif

	// These are the feature levels that we will accept.
	D3D_FEATURE_LEVEL featureLevels[] =
	{
		D3D_FEATURE_LEVEL_11_1,
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0,
		D3D_FEATURE_LEVEL_9_3,
		D3D_FEATURE_LEVEL_9_2,
		D3D_FEATURE_LEVEL_9_1
	};

    // This will be the feature level that 
    // is used to create our device and swap chain.
    D3D_FEATURE_LEVEL featureLevel;

    HRESULT hr = D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE,
        nullptr, createDeviceFlags, featureLevels, _countof(featureLevels),
        D3D11_SDK_VERSION, &swapChainDesc, &SwapChain, &Device, &featureLevel,
        &DeviceContext);

    if (hr == E_INVALIDARG)
    {
        hr = D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE,
            nullptr, createDeviceFlags, &featureLevels[1], _countof(featureLevels) - 1,
            D3D11_SDK_VERSION, &swapChainDesc, &SwapChain, &Device, &featureLevel,
            &DeviceContext);
    }

    if (FAILED(hr))
    {
        return -1;
    }

    // The Direct3D device and swap chain were successfully created.
    // Now we need to initialize the buffers of the swap chain.
    // Next initialize the back buffer of the swap chain and associate it to a 
    // render target view.
    ID3D11Texture2D* backBuffer;
    hr = SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&backBuffer);
    if (FAILED(hr))
    {
        return false;
    }

    hr = Device->CreateRenderTargetView(backBuffer, nullptr, &RenderTargetView);
    if (FAILED(hr))
    {
        return -1;
    }


    //DXCOMSafeRelease(backBuffer);

    // Create the depth buffer for use with the depth/stencil view.
    D3D11_TEXTURE2D_DESC depthStencilBufferDesc;
    ZeroMemory(&depthStencilBufferDesc, sizeof(D3D11_TEXTURE2D_DESC));

    depthStencilBufferDesc.ArraySize = 1;
    depthStencilBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
    depthStencilBufferDesc.CPUAccessFlags = 0; // No CPU access required.
    depthStencilBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
    depthStencilBufferDesc.Width = clientWidth;
    depthStencilBufferDesc.Height = clientHeight;
    depthStencilBufferDesc.MipLevels = 1;
    depthStencilBufferDesc.SampleDesc.Count = 1;
    depthStencilBufferDesc.SampleDesc.Quality = 0;
    depthStencilBufferDesc.Usage = D3D11_USAGE_DEFAULT;

    hr = Device->CreateTexture2D(&depthStencilBufferDesc, nullptr, &DepthStencilBuffer);
    if (FAILED(hr))
    {
        return -1;
    }

    hr = Device->CreateDepthStencilView(DepthStencilBuffer, nullptr, &DepthStencilView);
    if (FAILED(hr))
    {
        return -1;
    }

    // Setup depth/stencil state.
    D3D11_DEPTH_STENCIL_DESC depthStencilStateDesc;
    ZeroMemory(&depthStencilStateDesc, sizeof(D3D11_DEPTH_STENCIL_DESC));

    depthStencilStateDesc.DepthEnable = TRUE;
    depthStencilStateDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
    depthStencilStateDesc.DepthFunc = D3D11_COMPARISON_LESS;
    depthStencilStateDesc.StencilEnable = FALSE;

    hr = Device->CreateDepthStencilState(&depthStencilStateDesc, &DepthStencilState);

    // Setup rasterizer state.
    D3D11_RASTERIZER_DESC rasterizerDesc;
    ZeroMemory(&rasterizerDesc, sizeof(D3D11_RASTERIZER_DESC));

    rasterizerDesc.AntialiasedLineEnable = FALSE;
    rasterizerDesc.CullMode = D3D11_CULL_FRONT;
    rasterizerDesc.DepthBias = 0;
    rasterizerDesc.DepthBiasClamp = 0.0f;
    rasterizerDesc.DepthClipEnable = TRUE;
    rasterizerDesc.FillMode = D3D11_FILL_SOLID;
    rasterizerDesc.FrontCounterClockwise = FALSE;
    rasterizerDesc.MultisampleEnable = FALSE;
    rasterizerDesc.ScissorEnable = FALSE;
    rasterizerDesc.SlopeScaledDepthBias = 0.0f;

    // Create the rasterizer state object.
    hr = Device->CreateRasterizerState(&rasterizerDesc, &RasterizerState);
    if (FAILED(hr))
    {
        return false;
    }

    // Initialize the viewport to occupy the entire client area.
    Viewport.Width = static_cast<float>(clientWidth);
    Viewport.Height = static_cast<float>(clientHeight);
    Viewport.TopLeftX = 0.0f;
    Viewport.TopLeftY = 0.0f;
    Viewport.MinDepth = 0.0f;
    Viewport.MaxDepth = 1.0f;
    DeviceContext->RSSetViewports(1, &Viewport);
    DeviceContext->RSSetState(RasterizerState);

    DeviceContext->OMSetRenderTargets(1, &RenderTargetView, DepthStencilView);

    auto FDXAllocator = new D3D11FRIDynamicAllocator(this);
    dynamicAllocator = FDXAllocator;

    return true;

}

DXGI_RATIONAL D3D11FRIContext::QueryRefreshRate(UINT screenWidth, UINT screenHeight, BOOL vsync)
{
    DXGI_RATIONAL refreshRate = { 0, 1 };
    if (vsync)
    {
        TComPtr<IDXGIFactory> factory;
        TComPtr<IDXGIAdapter> adapter;
        TComPtr<IDXGIOutput> adapterOutput;
        DXGI_MODE_DESC* displayModeList;

        // Create a DirectX graphics interface factory.
        HRESULT hr = CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&factory);
        if (FAILED(hr))
        {
            MessageBox(0,
                TEXT("Could not create DXGIFactory instance."),
                TEXT("Query Refresh Rate"),
                MB_OK);

            throw new std::exception("Failed to create DXGIFactory.");
        }

        hr = factory->EnumAdapters(0, &adapter);
        if (FAILED(hr))
        {
            MessageBox(0,
                TEXT("Failed to enumerate adapters."),
                TEXT("Query Refresh Rate"),
                MB_OK);

            throw new std::exception("Failed to enumerate adapters.");
        }

        hr = adapter->EnumOutputs(0, &adapterOutput);
        if (FAILED(hr))
        {
            MessageBox(0,
                TEXT("Failed to enumerate adapter outputs."),
                TEXT("Query Refresh Rate"),
                MB_OK);

            throw new std::exception("Failed to enumerate adapter outputs.");
        }

        UINT numDisplayModes;
        hr = adapterOutput->GetDisplayModeList(DXGI_FORMAT_B8G8R8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numDisplayModes, nullptr);
        if (FAILED(hr))
        {
            MessageBox(0,
                TEXT("Failed to query display mode list."),
                TEXT("Query Refresh Rate"),
                MB_OK);

            throw new std::exception("Failed to query display mode list.");
        }

        displayModeList = new DXGI_MODE_DESC[numDisplayModes];
        assert(displayModeList);

        hr = adapterOutput->GetDisplayModeList(DXGI_FORMAT_B8G8R8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numDisplayModes, displayModeList);
        if (FAILED(hr))
        {
            MessageBox(0,
                TEXT("Failed to query display mode list."),
                TEXT("Query Refresh Rate"),
                MB_OK);

            throw new std::exception("Failed to query display mode list.");
        }

        // Now store the refresh rate of the monitor that matches the width and height of the requested screen.
        for (UINT i = 0; i < numDisplayModes; ++i)
        {
            if (displayModeList[i].Width == screenWidth && displayModeList[i].Height == screenHeight)
            {
                refreshRate = displayModeList[i].RefreshRate;
            }
        }

        delete[] displayModeList;
    }

    return refreshRate;
}

D3D11FRIContext::~D3D11FRIContext()
{
    SwapChain->SetFullscreenState(FALSE, NULL);

    delete win32Context;
}

void D3D11FRIContext::SwapBuffers()
{
    SwapChain->Present(0, 0);
    DeviceContext->ClearState();
}


void D3D11FRIContext::HandleEvents()
{
    MSG msg;
    while (PeekMessage(&msg, win32Context->hWindow, 0, 0, PM_REMOVE))
    {
        if (msg.message == WM_QUIT)
        {
            PollCloseEvent();
        }
        else
        {

            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }
}

void D3D11FRIContext::SetCursorPosition(FVector2 pos)
{
    win32Context->SetCursorPosition(pos.x, pos.y);
}


FVector2 D3D11FRIContext::GetCursorPosition()
{
    int x, y;
    win32Context->GetCursorPosition(&x, &y);

    return FVector2(x, y);
}

FViewportRect D3D11FRIContext::GetViewport()
{
    if (win32Context)
        return FViewportRect(0, 0, win32Context->Width, win32Context->Height);
    else
        return FViewportRect(0, 0, 0, 0);
}


void D3D11FRIContext::ShowCursor()
{
    while (::ShowCursor(TRUE) < 0);
}

void D3D11FRIContext::HideCursor()
{
    while (::ShowCursor(FALSE) >= 0);
}