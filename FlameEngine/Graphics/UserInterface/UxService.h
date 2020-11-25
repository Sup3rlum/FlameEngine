#pragma once


#include "../../dll/nchfx.h"


#include "UxContainer.h"


EXPORT_CLASS UxService
{

public:


	UxService();


	void PushContainer(UxContainer* container);
	UxContainer* PopContainer();
	UxContainer* CurrentContainer();


	void Render();
	void Update();

private:
	std::stack<UxContainer*> pCurrentContainer;

};

