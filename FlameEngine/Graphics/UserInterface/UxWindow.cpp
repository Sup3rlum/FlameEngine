#include "UxWindow.h"



UxWindow::UxWindow(STRING title) : UxWindow(title, UxRectangle(fVector2(0), fVector2(100)))
{

}

UxWindow::UxWindow(STRING title, UxRectangle bounds)
{

	Position = bounds.Position;
	Size = bounds.Size;

	WindowTitle = title;

	Background = new UxFrame(bounds.Position, bounds.Size);
	TitleLabel = new UxLabel(bounds.Position, WindowTitle, new UxFont("C:\\Windows\\Fonts\\arial.ttf", 16));

	TitleLabel->Size = fVector2(bounds.Size.x, TitleLabel->pFont->FontSize + 5);

	Background->color = Color(200, 200, 200, 200);
	Background->pxBorderRadius = 10;
}


void UxWindow::Render()
{
	Background->Render();
	TitleLabel->Render();
}


void UxWindow::Update()
{


	for (int i = 0; i < mChildren.size(); i++)
	{
		mChildren[0]->Update();
	}

	Background->Update();
	TitleLabel->Update();

	Background->Position = Position;
	Background->Size = Size;
	TitleLabel->Position = Position + fVector2(10);



	mTitleRectangle.Position = Position;
	mTitleRectangle.Size = fVector2(Size.x, TitleLabel->Size.y);

	bool prevDrag = dragging;

	if (GetParent()->pRenderingService->mAttachedContext->GetMouseButtonState(MouseButton::LEFT) == MouseButtonState::RELEASED)
	{
		dragging = false;
	}

	if (mTitleRectangle.Contains(GetParent()->GetCursorPosition()))
	{

		if (!prevDrag && GetParent()->pRenderingService->mAttachedContext->GetMouseButtonState(MouseButton::LEFT) == MouseButtonState::PRESSED)
		{
			dragging = true;
			dragDiff = GetParent()->GetCursorPosition() - Position;
		}
	}

	if (dragging)
	{
		Position = GetParent()->GetCursorPosition() - dragDiff;
	}
	
}
void UxWindow::SetParent(UxContainer* p)
{
	Background->SetParent(p);
	TitleLabel->SetParent(p);
	__super::SetParent(p);
}
