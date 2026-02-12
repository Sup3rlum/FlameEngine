
#define _CRTDBG_MAP_ALLOC

#include "SteelCircus.h"



#include <iostream>
#include <string>

#include <crtdbg.h>

int main()
{

	//std::cout << "Enter your resolution as \"width height\":\r\n";

	int width, height;

	//std::cin >> width >> height;

	width = 2560;
	height = 1440;

	//std::cout << "Full screen? (y/n):\r\n";

	char c = 'n';
	//std::cin >> c;


	FRIRenderingContextDescription contextDescription;
	contextDescription.Width = width;
	contextDescription.Height = height;
	contextDescription.SampleCount = 0;
	contextDescription.RenderFramework = EFRIRendererFramework::DX12;
	contextDescription.WinContext = NULL;
	contextDescription.BackBufferCount = 3;

	if (c == 'y')
		contextDescription.IsFullscreen = true;
	else if (c == 'n')
		contextDescription.IsFullscreen = false;
	else
		return 0;


	SteelCircus app(L"SteelCircus");
	app.CreateContext(contextDescription);
	
	if (!app.Content.Connect("./Assets/"))
	{
		return 0;
	}

	app.Load();
	app.Run();


	_CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_DEBUG);
	_CrtDumpMemoryLeaks();
	
	return 0;
}

