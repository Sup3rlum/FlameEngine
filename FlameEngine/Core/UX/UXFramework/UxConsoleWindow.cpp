#include "UxConsoleWindow.h"



UxConsoleWindow::UxConsoleWindow() : UxWindow("FlameEngine::Console")
{
	TextLine = new UxLabel(FVector2(0), "", new UxFont("C:\\Windows\\Fonts\\arial.ttf", 16));
	TextLine->mMultiline = true;
	TextLine->BackgroundColor = Color::Transparent;
}



void UxConsoleWindow::Render()
{
	__super::Render();

	TextLine->Render();
}
void UxConsoleWindow::Update()
{
	TextLine->Text = LoggerService::pHandlingInstance->data;
	TextLine->Position = Position + FVector2(0, TitleLabel->Size.y + 10) + FVector2(10);
	TextLine->Size = Size - FVector2(20, 40 + TitleLabel->Size.y);

	TextLine->Update();

	__super::Update();
}
void UxConsoleWindow::SetParent(UxContainer* p)
{
	TextLine->SetParent(p);
	__super::SetParent(p);
}
void UxConsoleWindow::ToggleConsole()
{
	SetVisibility(!GetVisibility());
}