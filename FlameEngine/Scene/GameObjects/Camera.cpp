#include "Camera.h"


Camera::Camera()
{

}
Camera::~Camera()
{

}

void Camera::Update()
{
	View = fMatrix4::CreateView(Position, Position + LookDirection, Up);
	DebugView = fMatrix4::CreateView(-LookDirection * 20.0f, fVector3(0), Up);


	fMatrix4 m2(fMatrix3::Transpose(fMatrix4::ToMatrix3(View)));
	m2[3][3] = 1.0f;

	//ViewInverse = fMatrix4::Translation(Position) * m2;
	ViewInverse = fMatrix4::Inverse(View);
	ProjectionInverse = fMatrix4::Inverse(Projection);
}

void Camera::GetFrustumCorners(fVector3* corners)
{



}