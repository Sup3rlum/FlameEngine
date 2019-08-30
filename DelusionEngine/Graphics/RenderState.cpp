#include "RenderState.h"

stack<RenderState> RenderState::States;

void RenderState::Push(RenderState* _state)
{
	States.push(*_state);
	ApplyState();
}
void RenderState::Pop()
{
	if (States.size() > 1)
	{
		States.pop();
		ApplyState();
	}
}

RenderState RenderState::Top()
{
	return States.top();
}

void RenderState::ApplyState()
{
	RenderState& top = States.top();

	glDepthFunc((int)top.DepthFunction);
	glCullFace((int)top.CullState);
	glBlendFunc((int)top.SourceBlend, (int)top.DestinationBlend);
}

void RenderState::InitilizeSurface()
{
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glEnable(GL_CULL_FACE);
}