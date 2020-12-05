#include "FpsCamera.h"

FpsCamera::FpsCamera(Context* _cont)
{
	_currentContext = _cont;

	horizontalAngle = -HALF_PI;
	verticalAngle = -QUARTER_PI;


	Projection = FMatrix4::CreatePerspective(ToRadians(Fov), 16.0f / 9.0f, 2.0f, 300.0f);


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


void FpsCamera::GetFrustumCorners(FVector3* corners)
{
	/*FVector4 ndc_corners[8] =
	{
		FVector4(1.0f, -1.0f, -1.0f, 1.0f),	 // llb
		FVector4(-1.0f, -1.0f, -1.0f, 1.0f), // lrb
		FVector4(-1.0f, 1.0f, -1.0f, 1.0f),  // urb
		FVector4(1.0f, 1.0f, -1.0f, 1.0f),	 // ulb


		FVector4(1.0f, -1.0f, 1.0f, 1.0f),   // llf
		FVector4(-1.0f, -1.0f, 1.0f, 1.0f),  // lrf
		FVector4(-1.0f, 1.0f, 1.0f, 1.0f),   // urf
		FVector4(1.0f, 1.0f, 1.0f, 1.0f)	 // ulf

	};

	
	FMatrix4 testttt = FMatrix4::Inverse(FMatrix4::CreatePerspective(ToRadians(Fov), 16.0f / 9.0f, 2.0f, 40.0f));

	FMatrix4 roview = ViewInverse;

	roview[0][3] = 0;
	roview[1][3] = 0;
	roview[2][3] = 0;

	roview[3][0] = 0;
	roview[3][1] = 0;
	roview[3][2] = 0;


	FMatrix4 inverseVPMatrix = FMatrix4::Transpose(roview * testttt);


	for (uint32_t i = 0; i < 8; i++)
	{
		FVector4 p = inverseVPMatrix * ndc_corners[i];

		p /= p.w;

		corners[i] = p.xyz;
	}*/

	__super::GetFrustumCorners(corners);

}