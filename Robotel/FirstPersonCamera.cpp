#include "FirstPersonCamera.h"

FirstPersonCamera::FirstPersonCamera(glm::vec3 position, glm::vec3 initialOffset, float yaw, float pitch, glm::vec3 up) : Camera(position, up, yaw, pitch)
{
	this->initialOffset = initialOffset;
}

void FirstPersonCamera::ProcessMouseMovement(float xoffset, float yoffset, glm::vec3& cameraOffset, GLboolean constrainPitch)
{
	xoffset *= mouseSensitivity;
	yaw += xoffset;
	if (yaw > YAW1D + 45)
		yaw = YAW1D + 45;
	else if (yaw < YAW1D -45)
		yaw = YAW1D - 45;
	updateCameraVectors();
}
