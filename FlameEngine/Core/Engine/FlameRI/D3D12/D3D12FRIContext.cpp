#include "D3D12FRIContext.h"
#include "D3D12FRICommon.h"

LRESULT CALLBACK D3D12FRIContext::Win32MessageHandler(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam)
{
    switch (umessage)
    {
        case WM_KEYDOWN:
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
        case WM_LBUTTONDOWN:    InputHandlerDelegate2(FMouseButton::Left, FKeyEvent::OnPress);    break;
        case WM_LBUTTONUP:      InputHandlerDelegate2(FMouseButton::Left, FKeyEvent::OnRelease);  break;
        case WM_RBUTTONDOWN:    InputHandlerDelegate2(FMouseButton::Right, FKeyEvent::OnPress);    break;
        case WM_RBUTTONUP:      InputHandlerDelegate2(FMouseButton::Right, FKeyEvent::OnRelease);  break;

        case WM_QUIT:
        case WM_DESTROY:
            PollCloseEvent();
            break;

        default:
            return DefWindowProc(hwnd, umessage, wparam, lparam);
    }
    return DefWindowProc(hwnd, umessage, wparam, lparam);
}


void D3D12FRIContext::Initialize()
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
        win32Context = new Win32Context(
            "Engine2 D3D12, Multithreaded", 
            PosX, 
            PosY, 
            InstanceDescription.Width, 
            InstanceDescription.Height, 
            InstanceDescription.IsFullscreen, 
            FWin32MessageProcDelegate::Make<D3D12FRIContext, &D3D12FRIContext::Win32MessageHandler>(this)
        );
    }
    isActive = true;

    if (!InitializeDX())
    {
        return;
    }

    win32Context->Show();
}


void D3D12FRIContext::GetAdapter(IDXGIFactory1* pFactory,
    IDXGIAdapter1** ppAdapter,
    bool requestHighPerformanceAdapter)
{
    *ppAdapter = nullptr;

    ComPtr<IDXGIAdapter1> adapter;
    ComPtr<IDXGIFactory6> factory6;
    if (SUCCEEDED(pFactory->QueryInterface(IID_PPV_ARGS(&factory6))))
    {
        for (
            UINT adapterIndex = 0;
            SUCCEEDED(factory6->EnumAdapterByGpuPreference(
                adapterIndex,
                requestHighPerformanceAdapter == true ? DXGI_GPU_PREFERENCE_HIGH_PERFORMANCE : DXGI_GPU_PREFERENCE_UNSPECIFIED,
                IID_PPV_ARGS(&adapter)));
            ++adapterIndex)
        {
            DXGI_ADAPTER_DESC1 desc;
            adapter->GetDesc1(&desc);

            if (desc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE)
            {
                // Don't select the Basic Render Driver adapter.
                // If you want a software adapter, pass in "/warp" on the command line.
                continue;
            }

            // Check to see whether the adapter supports Direct3D 12, but don't create the
            // actual device yet.
            if (SUCCEEDED(D3D12CreateDevice(adapter.Get(), D3D_FEATURE_LEVEL_11_0, _uuidof(ID3D12Device), nullptr)))
            {
                break;
            }
        }
    }

    if (adapter.Get() == nullptr)
    {
        for (UINT adapterIndex = 0; SUCCEEDED(pFactory->EnumAdapters1(adapterIndex, &adapter)); ++adapterIndex)
        {
            DXGI_ADAPTER_DESC1 desc;
            adapter->GetDesc1(&desc);

            if (desc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE)
            {
                // Don't select the Basic Render Driver adapter.
                // If you want a software adapter, pass in "/warp" on the command line.
                continue;
            }

            // Check to see whether the adapter supports Direct3D 12, but don't create the
            // actual device yet.
            if (SUCCEEDED(D3D12CreateDevice(adapter.Get(), D3D_FEATURE_LEVEL_11_0, _uuidof(ID3D12Device), nullptr)))
            {
                break;
            }
        }
    }

    *ppAdapter = adapter.Detach();
}

FD3D12Device* D3D12FRIContext::CreateDevice()
{
    ComPtr<ID3D12Device4> Device = 0;
    UINT dxgiFactoryFlags = 0;

#if defined(_DEBUG)
    // Enable the debug layer (requires the Graphics Tools "optional feature").
    // NOTE: Enabling the debug layer after device creation will invalidate the active device.
    {
        ComPtr<ID3D12Debug1> debugController;
        if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController))))
        {
            debugController->EnableDebugLayer();
            debugController->SetEnableGPUBasedValidation(TRUE);

            // Enable additional debug layers.
            dxgiFactoryFlags |= DXGI_CREATE_FACTORY_DEBUG;
        }
    }
#endif

    ComPtr<IDXGIFactory4> factory;
    ThrowIfFailed(CreateDXGIFactory2(dxgiFactoryFlags, IID_PPV_ARGS(&factory)));

    ComPtr<IDXGIAdapter1> hardwareAdapter;
    GetAdapter(factory.Get(), &hardwareAdapter, true);

    ThrowIfFailed(D3D12CreateDevice(
        hardwareAdapter.Get(),
        D3D_FEATURE_LEVEL_11_0,
        IID_PPV_ARGS(&Device)
    ));

    // Describe and create the command queue.
    ComPtr<ID3D12CommandQueue> CommandQueue;
    D3D12_COMMAND_QUEUE_DESC queueDesc = {};
    queueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
    queueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;

    ThrowIfFailed(Device->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(&CommandQueue)));
    //NAME_D3D12_OBJECT(CommandQueue);

    RECT clientRect;
    GetClientRect(win32Context->GetHWND(), &clientRect);

    uint32 clientWidth = clientRect.right - clientRect.left;
    uint32 clientHeight = clientRect.bottom - clientRect.top;

    // Describe and create the swap chain.
    ComPtr<IDXGISwapChain4> SwapChain;
    DXGI_SWAP_CHAIN_DESC1 swapChainDesc = {};
    swapChainDesc.BufferCount = InstanceDescription.BackBufferCount;
    swapChainDesc.Width = clientWidth;
    swapChainDesc.Height = clientHeight;
    swapChainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
    swapChainDesc.SampleDesc.Count = 1;

    ComPtr<IDXGISwapChain1> swapChain;
    ThrowIfFailed(factory->CreateSwapChainForHwnd(
        CommandQueue.Get(),        // Swap chain needs the queue so that it can force a flush on it.
        win32Context->GetHWND(),
        &swapChainDesc,
        nullptr,
        nullptr,
        &swapChain
    ));

    ThrowIfFailed(factory->MakeWindowAssociation(win32Context->GetHWND(), DXGI_MWA_NO_ALT_ENTER));

    ThrowIfFailed(swapChain.As(&SwapChain));
    CurrentBackBufferIndex = SwapChain->GetCurrentBackBufferIndex();


    return new FD3D12Device(Device, CommandQueue, SwapChain, InstanceDescription.BackBufferCount, this);
}

bool D3D12FRIContext::InitializeDX()
{
    Device = CreateDevice();
    dynamicAllocator = new D3D12FRIDynamicAllocator(Device);
    return true;

}

DXGI_RATIONAL D3D12FRIContext::QueryRefreshRate(UINT screenWidth, UINT screenHeight, BOOL vsync)
{
    DXGI_RATIONAL refreshRate = { 0, 1 };
    if (vsync)
    {
        ComPtr<IDXGIFactory> factory;
        ComPtr<IDXGIAdapter> adapter;
        ComPtr<IDXGIOutput> adapterOutput;
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

D3D12FRIContext::~D3D12FRIContext()
{
    Device->SwapChain->SetFullscreenState(FALSE, NULL);
    delete win32Context;
}

void D3D12FRIContext::SwapBuffers()
{
    Device->SwapChain->Present(0, 0);
}


bool D3D12FRIContext::HandleEvents()
{
    MSG msg;
    while (PeekMessage(&msg, win32Context->GetHWND(), 0, 0, PM_REMOVE))
    {
        if (msg.message == WM_QUIT)
            PollCloseEvent();

        TranslateMessage(&msg);
        DispatchMessage(&msg);

        return true;
    }
    return false;
}

void D3D12FRIContext::SetCursorPosition(FVector2 pos)
{
    win32Context->SetCursorPosition(pos.x, pos.y);
}


FVector2 D3D12FRIContext::GetCursorPosition()
{
    int x, y;
    win32Context->GetCursorPosition(&x, &y);

    return FVector2(x, y);
}

FViewportRect D3D12FRIContext::GetViewport()
{
    if (win32Context)
        return FViewportRect(0, 0, win32Context->Width, win32Context->Height);
    else
        return FViewportRect(0, 0, 0, 0);
}


void D3D12FRIContext::ShowCursor()
{
    while (::ShowCursor(TRUE) < 0);
}

void D3D12FRIContext::HideCursor()
{
    while (::ShowCursor(FALSE) >= 0);
}

FRICommandContext* D3D12FRIContext::GetCommandContext(uint32 ThreadIdx)
{
    return Device->GetCommandContext(ThreadIdx);
}

void D3D12FRIContext::CreateCommandContext()
{
    Device->CreateCommandContext();
}