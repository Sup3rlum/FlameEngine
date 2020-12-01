#include "FpsCamera.h"

FpsCamera::FpsCamera(Context* _cont)
{
	_currentContext = _cont;

	horizontalAngle = -HALF_PI;
	verticalAngle = -QUARTER_PI;


	Projection = fMatrix4::CreatePerspective(ToRadians(Fov), 16.0f / 9.0f, 2.0f, 1000.0f);

	Position = fVector3(5.0f, 5.0f, 5.0f);

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

	LookDirection = fVector3
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


void FpsCamera::GetFrustumCorners(fVector3* corners)
{
		fVector4 ndc_corners[8] =
		{
			fVector4(1.0f, -1.0f, -1.0f, 1.0f),	 // llb
			fVector4(-1.0f, -1.0f, -1.0f, 1.0f), // lrb
			fVector4(-1.0f, 1.0f, -1.0f, 1.0f),  // urb
			fVector4(1.0f, 1.0f, -1.0f, 1.0f),	 // ulb


			fVector4(1.0f, -1.0f, 1.0f, 1.0f),   // llf
			fVector4(-1.0f, -1.0f, 1.0f, 1.0f),  // lrf
			fVector4(-1.0f, 1.0f, 1.0f, 1.0f),   // urf
			fVector4(1.0f, 1.0f, 1.0f, 1.0f)	 // ulf

	};
	fMatrix4 inverseVPMatrix = fMatrix4::Transpose(ViewInverse * ProjectionInverse);


	for (uint32_t i = 0; i < 8; i++)
	{
		fVector4 p = inverseVPMatrix * ndc_corners[i];

		p /= p.w;

		corners[i] = p.xyz;
	}

}