#pragma once
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>


//read the docs


enum CameraMovement {
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT,
	DOWN,
	UP
};

const float YAW = -45.0f;
const float PITCH = -45.0f;
const float SPEED = 2.5f;
const float SENSITIVITY = 0.1f;
const float ZOOM = 45.0f;

class Camera
{
public:
	glm::vec3 position;
	glm::vec3 front;
	glm::vec3 up;
	glm::vec3 right;
	glm::vec3 worldUp;

	float yaw;
	float pitch;

	float moveSpeed;
	float mouseSensitivity;
	float zoom;

	Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = YAW, float pitch = PITCH);
	Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch);

	glm::mat4 GetViewMatrix();
	virtual void ProcessKeyboard(CameraMovement direction, float deltaTime);
	virtual void ProcessMouseMovement(float xoffset, float yoffset, glm::vec3& cameraOffset, GLboolean constrainPitch = true);
	void ProcessMouseScroll(float yoffset);

	void SetPosition(glm::vec3 position);
protected:
	virtual void updateCameraVectors();
};

