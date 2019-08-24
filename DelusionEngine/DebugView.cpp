#include "DebugView.h"

Shader* DebugView::_shader = NULL;
VertexBuffer* DebugView::_vb = NULL;
RenderBatch* DebugView::_renderBatch = NULL;
Font* DebugView::_debugFont = NULL;

int DebugView::_fps = 0;
char DebugView::_gpuName[256] = "";

void DebugView::Init(ContextDescription* _cont)
{

	_shader = new Shader("./shaders/debug_view.vert", "./shaders/debug_view.frag");

	_vb = new VertexBuffer(VertexColor::Elements);

	VertexColor _vData[] =
	{
		VertexColor(Vector3(0,0,0),Vector3(1,0,0)),
		VertexColor(Vector3(1,0,0),Vector3(1,0,0)),

		VertexColor(Vector3(0,0,0),Vector3(0,1,0)),
		VertexColor(Vector3(0,1,0),Vector3(0,1,0)),

		VertexColor(Vector3(0,0,0),Vector3(0,0,1)),
		VertexColor(Vector3(0,0,1),Vector3(0,0,1)),

	};

	_vb->SetData(_vData, 6);


	_renderBatch = new RenderBatch(_cont);
	_debugFont = new Font("..", 22);

	_cont->GetGraphicsID(_gpuName);
}

void DebugView::Draw(Camera* _cam)
{

	_shader->UseProgram();

	_shader->SetMatrix("View", _cam->DebugView);
	_shader->SetMatrix("Projection", _cam->Projection);
	_shader->SetMatrix("World", identity<Matrix4>());

	_vb->Render(GL_LINES);

	char a[256];
	char b[256];
	char c[256];

	sprintf_s(a, "FPS: %i", _fps);
	sprintf_s(b, "Position: X:%i Y:%i Z:%i", (int)_cam->Position.x, (int)_cam->Position.y, (int)_cam->Position.z);
	sprintf_s(c, "GPU: %s", glGetString(GL_RENDERER));

	_renderBatch->DrawString(a, _debugFont, 0, 20, Color::white);
	_renderBatch->DrawString(b, _debugFont, 0, 40, Color::white);
	//_renderBatch->DrawString(c, _debugFont, 0, 60, Color::white);

}
void DebugView::Update(FrameTime* _frTime)
{
	if (_frTime->EllapsedFrames % 1000 == 0)
		_fps = (int)(1.0 / _frTime->DeltaTime);
}