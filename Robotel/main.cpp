#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <vector>
#include <string>
#include <fstream>

#include "stb_image.h"
#include "Shader.h"
#include "Camera.h"
#include "TextureManager.h"
#include "Object.h"

using namespace std;
#pragma region Declaratii

glm::vec3 cubePositions[] = {
		glm::vec3(0.0f,  0.0f,  0.0f),
		glm::vec3(2.0f,  5.0f, -15.0f),
		glm::vec3(-1.5f, -2.2f, -2.5f),
		glm::vec3(-3.8f, -2.0f, -12.3f),
		glm::vec3(2.4f, -0.4f, -3.5f),
		glm::vec3(-1.7f,  3.0f, -7.5f),
		glm::vec3(1.3f, -2.0f, -2.5f),
		glm::vec3(1.5f,  2.0f, -2.5f),
		glm::vec3(1.5f,  0.2f, -1.5f),
		glm::vec3(-1.3f,  1.0f, -1.5f)
};

//informatii fereastra
float screenWidth = 800, screenHeight = 600;

//for speed normalization
float deltaTime = 0.0f;	// Time between current frame and last frame
float lastFrame = 0.0f; // Time of last frame

//for rotation of camera
float lastX = 400, lastY = 300;
bool firstMouse = true;

//iluminare
glm::vec3 lightPos(-0.5f, 3.0f, -4.0f);

//buffere si variabile de stare
unsigned int VBO, VAO, EBO;

bool wireframe = false;
bool grass = true;
float angle = 0;
bool night = false;

Camera* mainCamera;
Shader* directionalShader, * flashShader;
vector<Object*> dulapuri;
#pragma endregion

#pragma region Utilitare
void changeAllShaders(Shader* shader)
{
	for (int i = 0; i < 10; ++i)
	{
		dulapuri[i]->SetShader(shader);
	}
}
#pragma endregion

#pragma region Initializari

void initShaders()
{
	directionalShader = new Shader("./Shaders/vertex/lightVertex.vert", "./Shaders/fragment/directionalLight.frag");
	flashShader = new Shader("./Shaders/vertex/lightVertex.vert", "./Shaders/fragment/flashLight.frag");
}

void initStaticObjects()
{
	for (int i = 0; i < 10; ++i)
	{
		dulapuri.push_back(new Object("cargo", (night ? flashShader : directionalShader)));
		dulapuri[i]->SetPosition(glm::vec3(5, 5, 5 * i));
		dulapuri[i]->Rotate(30.0f * i, Axis::z);
		dulapuri[i]->SetScale(glm::vec3(0.01f, 0.01f, 0.01f));
	}
}

void initAll()
{
	initShaders();
	initStaticObjects();
	mainCamera = new Camera();
}
#pragma endregion

#pragma region GLFW Input & Callbacks

//resize render area after you resize the window
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	screenWidth = width;
	screenHeight = height;
	glViewport(0, 0, width, height);
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_1 && action == GLFW_PRESS)
	{
		if (!wireframe)
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		else
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		wireframe = !wireframe;
	}
	if (key == GLFW_KEY_R && action == GLFW_PRESS)
	{
		angle = (int)(angle + 10) % 360;
	}
	if (key == GLFW_KEY_L && action == GLFW_PRESS)
	{
		
		if (!night)
			changeAllShaders(flashShader);
		else
			changeAllShaders(directionalShader);
		night = !night;
	}
}

void mouse_callback(GLFWwindow* window, double xPos, double yPos)
{
	if (firstMouse) // initially set to true
	{
		lastX = xPos;
		lastY = yPos;
		firstMouse = false;
	}
	float xOffset = xPos - lastX;
	float yOffset = lastY - yPos;	//le iei invers fiindca cand dai in sus vrei sa te uiti in sus
	lastX = xPos;
	lastY = yPos;

	mainCamera->ProcessMouseMovement(xOffset, yOffset);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	mainCamera->ProcessMouseScroll(yoffset);
}

void processInput(GLFWwindow* window)
{
	float currentFrame = glfwGetTime();
	deltaTime = currentFrame - lastFrame;
	lastFrame = currentFrame;

	float cameraSpeed = 2.5f * deltaTime;
	//if the user presses ESC then close the app
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		mainCamera->ProcessKeyboard(FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		mainCamera->ProcessKeyboard(BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		mainCamera->ProcessKeyboard(LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		mainCamera->ProcessKeyboard(RIGHT, deltaTime);
}

#pragma endregion

#pragma region Desenare

void draw()
{
#pragma region Light&Camera Settings

	//program
	dulapuri[0]->GetShader()->setVec3("viewPos", mainCamera->position);
	
	//light
	dulapuri[0]->GetShader()->setVec3("light.ambient", 0.2f, 0.2f, 0.2f);
	dulapuri[0]->GetShader()->setVec3("light.diffuse", 0.7f, 0.7f, 0.7f);
	dulapuri[0]->GetShader()->setVec3("light.specular", 1.0f, 1.0f, 1.0f);
	dulapuri[0]->GetShader()->setVec3("light.direction", -0.2f, -1.0f, 10.0f);
	if (night)
	{
		dulapuri[0]->GetShader()->setVec3("light.position", lightPos);
		dulapuri[0]->GetShader()->setFloat("light.constant", 1.0f);
		dulapuri[0]->GetShader()->setFloat("light.linear", 0.09f);
		dulapuri[0]->GetShader()->setFloat("light.quadratic", 0.032f);

		dulapuri[0]->GetShader()->setVec3("light.position", mainCamera->position);
		dulapuri[0]->GetShader()->setVec3("light.direction", mainCamera->front);
		dulapuri[0]->GetShader()->setFloat("light.cutOff", glm::cos(glm::radians(12.5f)));
		dulapuri[0]->GetShader()->setFloat("light.outerCutOff", glm::cos(glm::radians(17.5f)));
	}	
	glm::mat4 view = mainCamera->GetViewMatrix();
	glm::mat4 projection;
	projection = glm::perspective(glm::radians(mainCamera->zoom), screenWidth / screenHeight, 0.1f, 100.0f);

	//set uniform variables
	int viewLoc = glGetUniformLocation(dulapuri[0]->GetShader()->id, "view");
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
	int projectionLoc = glGetUniformLocation(dulapuri[0]->GetShader()->id, "projection");
	glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));
#pragma endregion

#pragma region Objects Setup
	for (int i = 0; i < 10; ++i)
		dulapuri[i]->Draw();
#pragma endregion
}

#pragma endregion

#pragma region Dealocare
void deleteAll()
{
	BufferManager::DeleteBuffers();
	delete directionalShader;
	delete flashShader;
	delete mainCamera;
	dulapuri.clear();
}
#pragma endregion

int main()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	GLFWwindow* window = glfwCreateWindow(screenWidth, screenHeight, "Robotel", NULL, NULL);
	if (window == NULL)
	{
		cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glewInit();

	//set callbacks
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetKeyCallback(window, key_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	//initializare
	initAll();

	//keep the window open
	while (!glfwWindowShouldClose(window))
	{
		//input
		processInput(window);
		
		//rendering
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glEnable(GL_DEPTH_TEST);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		draw();

		//call events and swap buffers
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	//dealocare resources
	deleteAll();
	glfwTerminate();
	return 0;
}
