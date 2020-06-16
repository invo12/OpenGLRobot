#include "ThirdPersonCamera.h"
#include<iostream>
using namespace std;
ThirdPersonCamera::ThirdPersonCamera(glm::vec3 position,glm::vec3 initialOffset, float yaw, float pitch, glm::vec3 up) : Camera(position,up, yaw, pitch)
{
	this->initialOffset = initialOffset;
}

void ThirdPersonCamera::ProcessMouseMovement(float xoffset, float yoffset, glm::vec3& cameraOffset, GLboolean constrainPitch)
{
	xoffset *= mouseSensitivity;
	yaw += xoffset;
	//if (yaw > YAW3D + 45)
	//	yaw = YAW3D + 45;
	//else if (yaw < YAW3D -45)
	//	yaw = YAW3D - 45;
	cameraOffset = initialOffset + glm::vec3(-cos(glm::radians(yaw)), 0, -sin(glm::radians(yaw)));
	updateCameraVectors();
}

//void ThirdPersonCamera::updateCameraVectors()
//{
//	// calculate the new Front vector
//	glm::vec3 tmp;
//	// also re-calculate the Right and Up vector
//	right = glm::normalize(glm::cross(front, worldUp));  // normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
//	up = glm::normalize(glm::cross(right, front));
//}
