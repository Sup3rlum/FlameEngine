#include "UxService.h"


UxService::UxService()
{

}


void UxService::Render()
{
	CurrentContainer()->RenderComponents();
}
void UxService::Update()
{
	CurrentContainer()->UpdateComponents();

}


void UxService::PushContainer(UxContainer* container)
{
	if (container != NULL)
	{
		pCurrentContainer.push(container);
	}
}

UxContainer* UxService::CurrentContainer()
{
	return pCurrentContainer.top();
}


UxContainer* UxService::PopContainer()
{
	UxContainer* ptr = CurrentContainer();

	pCurrentContainer.pop();

	return ptr;
}