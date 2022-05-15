
#include "FlameCraft.h"

#include <iostream>
#include <string>

int main()
{
	CoInitialize(0);

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
	contextDescription.RenderFramework = EFRIRendererFramework::DX11;
	contextDescription.WinContext = NULL;

	if (c == 'y')
		contextDescription.IsFullscreen = true;
	else if (c == 'n')
		contextDescription.IsFullscreen = false;
	else
		return 0;


	FlameCraft app(L"Test");
	app.CreateContext(contextDescription);

	if (!app.Content.Connect("./Assets/"))
	{
		return 0;
	}

	app.Load();
	app.Run();


	return 0;
}

