#include "DebugView.h"

Shader* DebugView::_shader = NULL;
VertexBuffer* DebugView::_vb = NULL;

void DebugView::Init()
{

	_shader = new Shader("./shaders/debug_view.vert", "./shaders/debug_view.frag");

	_vb = new VertexBuffer(VertexColor::Elements);

	VertexColor _vData[] =
	{
		VertexColor(vec3(0,0,0),vec3(1,0,0)),
		VertexColor(vec3(1,0,0),vec3(1,0,0)),

		VertexColor(vec3(0,0,0),vec3(0,1,0)),
		VertexColor(vec3(0,1,0),vec3(0,1,0)),

		VertexColor(vec3(0,0,0),vec3(0,0,1)),
		VertexColor(vec3(0,0,1),vec3(0,0,1)),

	};

	_vb->SetData(_vData, 6);
}

void DebugView::Draw(Camera* _cam)
{

	_shader->UseProgram();

	_shader->SetMatrix("View", _cam->DebugView);
	_shader->SetMatrix("Projection", _cam->Projection);
	_shader->SetMatrix("World", identity<mat4>());

	_vb->Render(GL_LINES);

}