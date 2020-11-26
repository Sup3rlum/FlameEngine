#pragma once

#include "UxWindow.h"
#include "../../Output/LoggerService.h"


EXPORT_CLASS UxConsoleWindow : public UxWindow
{
public:

	UxConsoleWindow();

	void Update() override;
	void Render() override;

	void SetParent(UxContainer* p) override;


	void ToggleConsole();

	UxLabel* TextLine;
};

