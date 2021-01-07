#include "FirstPersonCamera.h"

FpsCamera::FpsCamera()
{
	_currentContext = _cont;

	horizontalAngle = -HALF_PI;
	verticalAngle = -QUARTER_PI;


	Projection = FPerspectiveMatrix(ToRadians(Fov), 16.0f / 9.0f, 2.0f, 300.0f);


	Position = FVector3(5.0f, 5.0f, 5.0f);

	cursordLocked = true;
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



	if (cursordLocked)
	{

		_currentContext->SetCursorPosition
		(
			_currentContext->_contextDescription->width / 2.0,
			_currentContext->_contextDescription->height / 2.0
		);

		horizontalAngle += mouseSpeed /** _frTime->DeltaTime*/ * 0.003f * (_currentContext->_contextDescription->width / 2.0f - (float)xpos);
		verticalAngle += mouseSpeed /** _frTime->DeltaTime*/ * 0.003f * (_currentContext->_contextDescription->height / 2.0f - (float)ypos);
	}

	LookDirection = FVector3
	(
		cos(verticalAngle) * sin(horizontalAngle),
		sin(verticalAngle),
		cos(verticalAngle) * cos(horizontalAngle)
	);

	Right = FVector3
	(
		sin(horizontalAngle - HALF_PI),
		0,
		cos(horizontalAngle - HALF_PI)
	);

	Up = Right ^ LookDirection;

	if (_currentContext->GetKeyState(Keys::W) == KeyState::PRESSED)
	{
		Position += LookDirection * (float)FrameTime::FrameDeltaTime.count() * flySpeed;
	}
	if (_currentContext->GetKeyState(Keys::S) == KeyState::PRESSED)
	{
		Position -= LookDirection * (float)FrameTime::FrameDeltaTime.count() * flySpeed;
	}
	if (_currentContext->GetKeyState(Keys::D) == KeyState::PRESSED)
	{
		Position += Right * (float)FrameTime::FrameDeltaTime.count() * flySpeed;
	}
	if (_currentContext->GetKeyState(Keys::A) == KeyState::PRESSED)
	{
		Position -= Right * (float)FrameTime::FrameDeltaTime.count() * flySpeed;
	}

	__super::Update();
}

void FpsCamera::SetCursorLocked(bool b)
{
	cursordLocked = b;
	_currentContext->SetCursorVisible(!b);
}
