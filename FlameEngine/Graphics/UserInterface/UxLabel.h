#pragma once

#include "UxComponent.h"
#include "UxFrame.h"
#include "UxFont.h"

#include "../../Mathematics/Module.h"


EXPORT(class,  UxLabel) : public UxComponent
{
public:

	UxLabel(fVector2 position, STRING text, UxFont* font);

	STRING Text;
	fVector2 Position;
	fVector2 Size;


	void Render() override;
	void Update() override;


	void SetParent(UxContainer* parent) override;


	Color TextColor;
	Color BackgroundColor;
	UxFont* pFont;
	UxFrame* pBackgroundFrame;
	Shader* mLabelShader;

	bool mMultiline;

};

