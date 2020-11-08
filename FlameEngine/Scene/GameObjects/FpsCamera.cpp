#include "FpsCamera.h"

FpsCamera::FpsCamera(Context* _cont)
{
	_currentContext = _cont;

	horizontalAngle = -half_pi<float>();
	verticalAngle = -quarter_pi<float>();


	Projection = perspective(radians(Fov), 16.0f / 9.0f, 0.1f, 1000.0f);


	ProjectionInverse = inverse(Projection);

	Position = fVector3(5.0f, 5.0f, 5.0f);

	
}

void FpsCamera::Update()
{

	if (verticalAngle > half_pi<float>() - 0.1f)
	{
		verticalAngle = half_pi<float>() - 0.1f;
	}
	else if (verticalAngle < -half_pi<float>() + 0.1f)
	{
		verticalAngle = -half_pi<float>() + 0.1f;
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
		sin(horizontalAngle - half_pi<float>()),
		0,
		cos(horizontalAngle - half_pi<float>())
	);

	Up = cross(Right, Target);

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


	View = lookAt(Position, Position + Target, Up);
	DebugView = lookAt(-Target * 20.0f, fVector3(0), Up);
}