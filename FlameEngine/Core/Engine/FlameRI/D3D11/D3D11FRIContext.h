#pragma once


#include "Platform/Windows/Win32Context.h"
#include "../FRIContext.h"
#include "D3D11.h"

class D3D11FRIContext : public FRIContext
{
public:
	D3D11FRIContext(FRIRenderingContextDescription description, FRIContext* contextToCopy = NULL) :
		FRIContext(description, contextToCopy),
		win32Context(description.WinContext)
	{
	}

	~D3D11FRIContext();

	void Initialize();
	bool InitializeDX();

	void* GetPlatformSpecificHandle() { return win32Context; }
	void SwapBuffers();
	bool HandleEvents();
	FVector2 GetCursorPosition();
	void SetCursorPosition(FVector2 pos);
	FViewportRect GetViewport();


	void ShowCursor();
	void HideCursor();

	LRESULT Win32MessageHandler(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam);


	DXGI_RATIONAL QueryRefreshRate(UINT screenWidth, UINT screenHeight, BOOL vsync);

	friend class D3D11FRIDynamicAllocator;
	Win32Context* win32Context;


	ComPtr<ID3D11Device> Device = nullptr;
	ComPtr<ID3D11DeviceContext> DeviceContext = nullptr;
	ComPtr<IDXGISwapChain> SwapChain = nullptr;

	ComPtr<ID3D11RenderTargetView> RenderTargetView = nullptr;
	ComPtr<ID3D11DepthStencilView> DepthStencilView = nullptr;
	ComPtr<ID3D11Texture2D> DepthStencilBuffer = nullptr;

	ComPtr<ID3D11DepthStencilState> DepthStencilState = nullptr;
	ComPtr<ID3D11RasterizerState> RasterizerState = nullptr;
	D3D11_VIEWPORT Viewport = { 0 };

};