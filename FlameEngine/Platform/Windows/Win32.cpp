#include "Win32Context.h"



LRESULT CALLBACK Win32Context::Win32MessageProcSignature(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam)
{

	Win32Context* pInstance;

    if (umessage == WM_NCCREATE)
    {
        pInstance = static_cast<Win32Context*>(reinterpret_cast<CREATESTRUCT*>(lparam)->lpCreateParams);

        SetLastError(0);
        if (!SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pInstance)))
        {
            if (GetLastError() != 0)
                return FALSE;
        }
    }
    else
    {
        pInstance = reinterpret_cast<Win32Context*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
    }

	if (pInstance)
	{
		return pInstance->MessageHandler.Handler(hwnd, umessage, wparam, lparam);
	}

	return DefWindowProc(hwnd, umessage, wparam, lparam);
}