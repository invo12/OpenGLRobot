#pragma once
#include "Camera.h"

const float YAW3D = 90.0f;
const float PITCH3D = -30.0f;

class ThirdPersonCamera :
	public Camera
{
private:
	glm::vec3 initialOffset;
public:
	ThirdPersonCamera(glm::vec3 position ,glm::vec3 initialOffset, float yaw = YAW3D, float pitch = PITCH3D, glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f));
	void ProcessMouseMovement(float xoffset, float yoffset, glm::vec3& cameraOffset, GLboolean constrainPitch = true) override;
	//void updateCameraVectors() override;
};

