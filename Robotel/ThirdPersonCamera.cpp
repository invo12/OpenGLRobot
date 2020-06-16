#include "ThirdPersonCamera.h"

ThirdPersonCamera::ThirdPersonCamera(glm::vec3 position, float yaw, float pitch, glm::vec3 up) : Camera(position,up, yaw, pitch)
{
}

void ThirdPersonCamera::ProcessKeyboard(CameraMovement direction, float deltaTime, float canMove)
{
	if (canMove)
	{
		Camera::ProcessKeyboard(direction, deltaTime);
	}
}

void ThirdPersonCamera::ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch)
{
}

void ThirdPersonCamera::updateCameraVectors()
{
	front = glm::vec3(0, 0, 1);
	right = glm::vec3(-1, 0, 0);
	up = glm::normalize(glm::cross(right, front));
}
