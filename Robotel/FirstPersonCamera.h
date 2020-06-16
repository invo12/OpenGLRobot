#pragma once
#include "Camera.h"

const float YAW1D = 90;
const float PITCH1D = 0;

class FirstPersonCamera :
	public Camera
{
public:
	FirstPersonCamera(glm::vec3 position, glm::vec3 initialOffset, float yaw = YAW1D, float pitch = PITCH1D, glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f));
	void ProcessMouseMovement(float xoffset, float yoffset, glm::vec3& cameraOffset, GLboolean constrainPitch = true) override;
};

