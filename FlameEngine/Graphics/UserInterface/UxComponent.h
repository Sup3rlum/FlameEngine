#pragma once

#include "../../dll/nchfx.h"

#include "UxContainer.h"

EXPORT_CLASS UxComponent
{
public:

	UxComponent();

	virtual UxContainer* GetParent();
	virtual void SetParent(UxContainer* parent);


	virtual void Render();
	virtual void Update();
	RenderState* mRenderState;


	virtual void SetVisibility(bool vis);
	virtual bool GetVisibility();

private:
	UxContainer* mParent;
	bool mVisible;

	friend class UxContainer;
};

