
#include "TestGame.h"

#include <iostream>
#include <string>

int main()
{

	TestGameApplication app(L"Test", EFRIRendererFramework::DX11);
	
	if (!app.Content.Connect("./Assets/"))
	{
		return 0;
	}

	app.Load();
	app.Run();
	

	return 0;
}

