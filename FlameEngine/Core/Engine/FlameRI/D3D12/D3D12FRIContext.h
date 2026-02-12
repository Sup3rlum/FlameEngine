#pragma once



#include "Platform/Windows/Win32Context.h"
#include "../FRIContext.h"
#include "D3D12FRICommon.h"

class D3D12FRIContext : public FRIContext
{
private:
	void GetAdapter(IDXGIFactory1* pFactory,
		IDXGIAdapter1** ppAdapter,
		bool requestHighPerformanceAdapter);


public:
	D3D12FRIContext(FRIRenderingContextDescription description, FRIContext* contextToCopy = NULL) :
		FRIContext(description, contextToCopy),
		win32Context(description.WinContext)
	{
	}

	~D3D12FRIContext();

	void Initialize();
	bool InitializeDX();

	void* GetPlatformSpecificHandle() { return win32Context; }
	void SwapBuffers();
	bool HandleEvents();
	FVector2 GetCursorPosition();
	void SetCursorPosition(FVector2 pos);
	FViewportRect GetViewport();
	FD3D12Device* CreateDevice();
	FRICommandContext* GetCommandContext(uint32 ThreadIdx);
	void CreateCommandContext();

	void ShowCursor();
	void HideCursor();

	LRESULT Win32MessageHandler(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam);
	DXGI_RATIONAL QueryRefreshRate(UINT screenWidth, UINT screenHeight, BOOL vsync);

	friend class D3D11FRIDynamicAllocator;
	Win32Context* win32Context;
	FD3D12Device* Device;

	
	UINT RTVDescriptorSize;
	UINT CurrentBackBufferIndex;

	ComPtr<ID3D12Fence> Fence;
	uint64 fenceValue;
	uint64 FrameFenceValues[3] = { 0 };
	HANDLE FenceEvent;
	

};