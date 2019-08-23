#include "Camera.h"


Camera::Camera()
{
	horizontalAngle = -half_pi<float>();
	verticalAngle = -quarter_pi<float>();

	Projection = perspective(radians(Fov), 16.0f / 9.0f, 0.1f, 1000.0f);
	ProjectionInverse = inverse(Projection);

	Position = vec3(40.0f, 40.0f, 40.0f);
}
Camera::~Camera()
{

}

void Camera::Update(GLFWwindow* _window, ContextParameters* _params, FrameTime* _frTime)
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

	glfwGetCursorPos(_window, &xpos, &ypos);

	glfwSetCursorPos(
		_window, 
		_params->width / 2.0, 
		_params->height / 2.0);

	horizontalAngle += mouseSpeed /** _frTime->DeltaTime*/* 0.003f * (_params->width / 2.0 - xpos);
	verticalAngle	+= mouseSpeed /** _frTime->DeltaTime*/* 0.003f * (_params->height / 2.0 - ypos);

	Target = vec3
	(
		cos(verticalAngle) * sin(horizontalAngle),
		sin(verticalAngle),
		cos(verticalAngle) * cos(horizontalAngle)
	);

	Right = vec3
	(
		sin(horizontalAngle - half_pi<float>()),
		0,
		cos(horizontalAngle - half_pi<float>())
	);

	Up = cross(Right, Target);

	if (glfwGetKey(_window, GLFW_KEY_W) == GLFW_PRESS) 
	{
		Position += Target * (float)_frTime->DeltaTime * flySpeed;
	}
	if (glfwGetKey(_window, GLFW_KEY_S) == GLFW_PRESS) 
	{
		Position -= Target * (float)_frTime->DeltaTime * flySpeed;
	}
	if (glfwGetKey(_window, GLFW_KEY_D) == GLFW_PRESS) 
	{
		Position += Right * (float)_frTime->DeltaTime * flySpeed;
	}
	if (glfwGetKey(_window, GLFW_KEY_A) == GLFW_PRESS) 
	{
		Position -= Right * (float)_frTime->DeltaTime * flySpeed;
	}

	View = lookAt(Position, Position + Target, Up);
	DebugView = lookAt(-Target * 20.0f, vec3(0), Up);
}