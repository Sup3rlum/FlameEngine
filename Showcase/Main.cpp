
#include "TestGame.h"

#include <iostream>

int main()
{
	TestGameApplication app(L"Test", EFRIRendererFramework::DX11);
	app.Load();
	app.Run();
	

	return 0;
}