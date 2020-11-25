#include "UxLabel.h"


UxLabel::UxLabel(fVector2 position, STRING text, UxFont* font)
	:
	Position(position),
	Text(text),
	pFont(font)
{
	TextColor = Color::Black;
	BackgroundColor = Color::Transparent;

	pBackgroundFrame = new UxFrame(Position, Size);


	pFont = font;	

	mLabelShader = Shader::FromSource("./shaders/renderbatch_string.vert", "./shaders/renderbatch_string.frag");
	mMultiline = false;

}

void UxLabel::Render()
{
	pBackgroundFrame->Render();


	RenderState::Push(mRenderState);

	mLabelShader->UseProgram();

	mLabelShader->SetVector("Color", TextColor);
	mLabelShader->SetMatrix("View", GetParent()->pRenderingService->mView);

	int x = Position.x;
	int y = Position.y + pFont->FontSize;

	for (int c = 0; c < Text.length() && y < Position.y + Size.y; c++)
	{

		UxCharacter ch = pFont->Characters[Text[c]];


		if (x + (ch.Advance >> 6) >= Position.x + Size.x || Text[c] == '\n')
		{
			if (mMultiline)
			{

				x = Position.x;
				y += pFont->FontSize;
			}
		}
		else
		{
			mLabelShader->SetMatrix(
				"MatrixTransforms",
				fMatrix4::Translation
				(

					fVector3(x + ch.Bearing.x, y - ch.Bearing.y, 0)
				)
				*
				fMatrix4::Scaling
				(
					fVector3(ch.Size.x, ch.Size.y, 1)
				)
			);
			mLabelShader->SetTexture(0, ch.texture);

			if (Text[c] != '\n')
			{
				GetParent()->pRenderingService->DrawElement();
			}
		}

		if (Text[c] != '\n')
		{
			x += (ch.Advance >> 6);
		}
	}

	RenderState::Pop();
}
void UxLabel::SetParent(UxContainer* p)
{
	pBackgroundFrame->SetParent(p);
	__super::SetParent(p);
}
void UxLabel::Update()
{
	pBackgroundFrame->Size = Size;
	pBackgroundFrame->color = BackgroundColor;
	pBackgroundFrame->Position = Position;

	pBackgroundFrame->Update();
}