

#include "TestGame.h"



int main()
{

	printf("Hello!\n");


	TestGameApplication app(L"Test");
	app.Load();
	app.Run();
	
	printf("We are done\n");

	return 0;
}