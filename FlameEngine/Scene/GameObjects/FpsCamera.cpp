#include "FpsCamera.h"

FpsCamera::FpsCamera(Context* _cont)
{
	_currentContext = _cont;

	horizontalAngle = -HALF_PI;
	verticalAngle = -QUARTER_PI;


	Projection = fMatrix4::CreatePerspective(ToRadians(Fov), 16.0f / 9.0f, 1.0f, 100.0f);


	ProjectionInverse = fMatrix4::Inverse(Projection);

	Position = fVector3(5.0f, 5.0f, 5.0f);

	
}

void FpsCamera::Update()
{

	if (verticalAngle > HALF_PI - 0.1f)
	{
		verticalAngle = HALF_PI - 0.1f;
	}
	else if (verticalAngle < -HALF_PI + 0.1f)
	{
		verticalAngle = -HALF_PI + 0.1f;
	}


	double xpos, ypos;

	_currentContext->GetCursorPosition(&xpos, &ypos);


	_currentContext->SetCursorPosition
	(
		_currentContext->_contextDescription->width / 2.0,
		_currentContext->_contextDescription->height / 2.0
	);

	horizontalAngle += mouseSpeed /** _frTime->DeltaTime*/ * 0.003f * (_currentContext->_contextDescription->width / 2.0f - (float)xpos);
	verticalAngle += mouseSpeed /** _frTime->DeltaTime*/ * 0.003f * (_currentContext->_contextDescription->height / 2.0f - (float)ypos);


	Target = fVector3
	(
		cos(verticalAngle) * sin(horizontalAngle),
		sin(verticalAngle),
		cos(verticalAngle) * cos(horizontalAngle)
	);

	Right = fVector3
	(
		sin(horizontalAngle - HALF_PI),
		0,
		cos(horizontalAngle - HALF_PI)
	);

	Up = Right ^ Target;

	if (_currentContext->GetKeyState(Keys::W) == KeyState::PRESSED)
	{
		Position += Target * (float)FrameTime::FrameDeltaTime.count() * flySpeed;
	}
	if (_currentContext->GetKeyState(Keys::S) == KeyState::PRESSED)
	{
		Position -= Target * (float)FrameTime::FrameDeltaTime.count() * flySpeed;
	}
	if (_currentContext->GetKeyState(Keys::D) == KeyState::PRESSED)
	{
		Position += Right * (float)FrameTime::FrameDeltaTime.count() * flySpeed;
	}
	if (_currentContext->GetKeyState(Keys::A) == KeyState::PRESSED)
	{
		Position -= Right * (float)FrameTime::FrameDeltaTime.count() * flySpeed;
	}


	View = fMatrix4::CreateView(Position, Position + Target, Up);
	DebugView = fMatrix4::CreateView(-Target * 20.0f, fVector3(0), Up);



	fMatrix3 m = fMatrix4::ToMatrix3(View);

	m = fMatrix3::Transpose(m);

	fMatrix4 m2(m);

	m2[3][3] = 1.0f;

	ViewInverse = fMatrix4::Translation(Position) * m2;

}