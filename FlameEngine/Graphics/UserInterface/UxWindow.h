#pragma once


#include "UxFrame.h"
#include "UxLabel.h"

EXPORT(class,  UxWindow) : public UxComponent
{
public:


	UxWindow(STRING title);
	UxWindow(STRING title, UxRectangle bounds);

	virtual void Render();
	virtual void Update();

	virtual void SetParent(UxContainer* parent) override;


	STRING WindowTitle;

	fVector2 Position;
	fVector2 Size;

	UxRectangle mTitleRectangle;

	UxFrame* Background;
	UxLabel* TitleLabel;
	std::vector<UxComponent*> mChildren;
	bool dragging;

	fVector2 dragDiff;

};

