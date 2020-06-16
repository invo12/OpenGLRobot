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
#include "FirstPersonCamera.h"
#include "ThirdPersonCamera.h"
#include "Camera.h"
#include "TextureManager.h"
#include "Object.h"
#include "Raycast.h"

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

int currentCamera = 0;
glm::vec3 cameraOffsets[3];
Camera* cameras[3];

Shader* directionalShader, * flashShader, *raycastShader;
Object* player;
vector<Object*> scena;
float playerSpeed = 1.0f;
#pragma endregion

#pragma region Utilitare
void changeAllShaders(Shader* shader)
{
	for (int i = 0; i < scena.size(); ++i)
	{
		scena[i]->SetShader(shader);
	}
}
//returned if you can move
void moveAndCheckCollision(glm::vec3 delta)
{
	delta = delta * glm::vec3(playerSpeed * deltaTime);
	player->Translate(delta);
	bool revert = false;
	for (int i = 0; i < scena.size(); ++i)
		if (player != scena[i] && player->GetCollider()->Intersects(*scena[i]->GetCollider()))
		{
			revert = true;
			break;
		}
	if (revert)
		player->Translate(-delta);
}
#pragma endregion

#pragma region Initializari

void initShaders()
{
	directionalShader = new Shader("./Shaders/vertex/lightVertex.vert", "./Shaders/fragment/directionalLight.frag");
	flashShader = new Shader("./Shaders/vertex/lightVertex.vert", "./Shaders/fragment/flashLight.frag");
	raycastShader = new Shader("./Shaders/vertex/raycast.vert", "./Shaders/fragment/raycast.frag");
}

void initStaticObjects()
{
	float tileScaleX = 0.1f;
	float tileScaleZ = 0.1f;
	float tileScaleY = 0.001f;
	int tileNumX = 15;
	int tileNumZ = 15;
	for (int i = 0; i < tileNumX; ++i) {
		for (int j = 0; j < tileNumZ; ++j) {
			scena.push_back(new Object("Assets/iarba", (night ? flashShader : directionalShader)));
			scena[scena.size() - 1]->SetScale(glm::vec3(tileScaleX, tileScaleY, tileScaleZ));
			scena[scena.size() - 1]->SetPosition(glm::vec3(i * 2.0f * tileScaleX, 0, j * 2.0f * tileScaleZ));
		}
	}
	for (float height = 0.1f; height <= 0.3f; height += 0.2f) {
		for (int i = 0; i < 2; ++i) {
			for (int j = 0; j < tileNumX; ++j) {
				scena.push_back(new Object("Assets/wall", (night ? flashShader : directionalShader)));
				if (i == 0) {
					scena[scena.size() - 1]->SetPosition(glm::vec3(j * 2.0f * tileScaleX, height, -tileScaleZ));
				}
				else {
					scena[scena.size() - 1]->SetPosition(glm::vec3(j * 2.0f * tileScaleX, height, 2.0f * tileNumZ * tileScaleZ - tileScaleZ));
				}
				scena[scena.size() - 1]->SetRotation(glm::vec3(0, 0, 90));
				scena[scena.size() - 1]->SetScale(glm::vec3(0.1f, 0.1f, 0.001f));
			}
		}
		for (int i = 0; i < 2; ++i) {
			for (int j = 0; j < tileNumZ; ++j) {
				if (i == 0) {
					scena.push_back(new Object("Assets/wall", (night ? flashShader : directionalShader)));
					scena[scena.size() - 1]->SetPosition(glm::vec3(-tileScaleX, height, j * 2.0f * tileScaleZ));
				}
				else {
					scena.push_back(new Object("Assets/wall", (night ? flashShader : directionalShader)));
					scena[scena.size() - 1]->SetPosition(glm::vec3(2.0f * tileNumX * tileScaleX - tileScaleX, height, j * 2.0f * tileScaleZ));
				}
				scena[scena.size() - 1]->SetRotation(glm::vec3(0, 90, 90));
				scena[scena.size() - 1]->SetScale(glm::vec3(0.1f, 0.1f, 0.001f));
			}
		}
	}
	scena.push_back(new Object("Assets/cabinet", (night ? flashShader : directionalShader)));
	scena[scena.size() - 1]->SetScale(glm::vec3(0.06f, 0.06f, 0.06f));
	scena[scena.size() - 1]->SetRotation(glm::vec3(0, 0, 0));
	scena[scena.size() - 1]->SetPosition(glm::vec3(0, 0, 0));

	scena.push_back(new Object("Assets/cargo", (night ? flashShader : directionalShader)));
	scena[scena.size() - 1]->SetScale(glm::vec3(0.0015f, 0.0015f, 0.0015f));
	scena[scena.size() - 1]->SetRotation(glm::vec3(0, 0, 0));
	scena[scena.size() - 1]->SetPosition(glm::vec3(2.3f, 0, 0.3f));

	scena.push_back(new Object("Assets/cargo", (night ? flashShader : directionalShader)));
	scena[scena.size() - 1]->SetScale(glm::vec3(0.0015f, 0.0015f, 0.0015f));
	scena[scena.size() - 1]->SetRotation(glm::vec3(0, 0, 0));
	scena[scena.size() - 1]->SetPosition(glm::vec3(1.3f, 2.0f, 0.7f));
	player = scena[scena.size() - 1];
	player->SetLayer(Layer::IgnoreRaycast);

	scena.push_back(new Object("Assets/WoodenCrate", (night ? flashShader : directionalShader)));
	scena[scena.size() - 1]->SetScale(glm::vec3(0.1f, 0.1f, 0.1f));
	scena[scena.size() - 1]->SetRotation(glm::vec3(0, 0, 0));
	scena[scena.size() - 1]->SetPosition(glm::vec3(0.2, 0, 2.7f));

	scena.push_back(new Object("Assets/roaba", (night ? flashShader : directionalShader)));
	scena[scena.size() - 1]->SetScale(glm::vec3(0.2f, 0.2f, 0.2f));
	scena[scena.size() - 1]->SetRotation(glm::vec3(0, -0, 0));
	scena[scena.size() - 1]->SetPosition(glm::vec3(0.3f, 0, 1.7f));
}

void initAll()
{
	initShaders();
	initStaticObjects();

	cameraOffsets[0] = glm::vec3(0, 1, 0);
	cameraOffsets[1] = glm::vec3(0, 0.2f, 1.25f);
	cameraOffsets[2] = glm::vec3(0, 1, 0);
	cameras[0] = new ThirdPersonCamera(player->GetPosition(), cameraOffsets[0]);
	cameras[1] = new FirstPersonCamera(player->GetPosition(), cameraOffsets[1]);
	cameras[2] = new Camera(glm::vec3(0, 2, 3));

	cameras[currentCamera]->ProcessMouseMovement(0, 0, cameraOffsets[currentCamera]);
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
	if (key == GLFW_KEY_C && action == GLFW_PRESS)
	{
		currentCamera = (currentCamera + 1) % 3;
	}
	if (key == GLFW_KEY_SPACE && action == GLFW_PRESS)
	{
		Object* hit = Raycast::castRay(player->GetPosition(), glm::vec3(0, 0, 1), 5, &scena);
		if (hit != nullptr)
		{
			hit->SetActive(false);
		}
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

	cameras[currentCamera]->ProcessMouseMovement(xOffset, yOffset, cameraOffsets[currentCamera]);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	cameras[currentCamera]->ProcessMouseScroll(yoffset);
}

void processInput(GLFWwindow* window)
{
	float currentFrame = glfwGetTime();
	deltaTime = currentFrame - lastFrame;
	if (deltaTime > 1)
		deltaTime = 0.01f;
	lastFrame = currentFrame;

	//if the user presses ESC then close the app
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
	{
		moveAndCheckCollision(glm::vec3(0, 0, 1));
	}
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
	{
		moveAndCheckCollision(glm::vec3(0, 0, -1));
	}
	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
	{
		moveAndCheckCollision(glm::vec3(-1, 0, 0));
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
	{
		moveAndCheckCollision(glm::vec3(1, 0, 0));
	}

	if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS)
	{
		moveAndCheckCollision(glm::vec3(0, 1, 0));
	}
	if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS)
	{
		moveAndCheckCollision(glm::vec3(0, -1, 0));
	}
	//camera calculations after moving the player
	if (currentCamera == 2)
	{
		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
			cameras[currentCamera]->ProcessKeyboard(FORWARD, deltaTime);
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
			cameras[currentCamera]->ProcessKeyboard(BACKWARD, deltaTime);
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
			cameras[currentCamera]->ProcessKeyboard(LEFT, deltaTime);
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
			cameras[currentCamera]->ProcessKeyboard(RIGHT, deltaTime);
	}
}
#pragma endregion

#pragma region Desenare&Update
void update() 
{
	moveAndCheckCollision(glm::vec3(0, -1, 0));
	if (currentCamera != 2)
	{
		cameras[currentCamera]->SetPosition(player->GetPosition() + cameraOffsets[currentCamera]);
	}

}
void draw()
{
#pragma region Light&Camera Settings

	for (int i = 0; i < scena.size(); ++i)
	{
		if (scena[i]->IsActive())
		{
			//program
			scena[i]->GetShader()->setVec3("viewPos", cameras[currentCamera]->position);

			//light
			scena[i]->GetShader()->setVec3("light.ambient", 0.2f, 0.2f, 0.2f);
			scena[i]->GetShader()->setVec3("light.diffuse", 0.7f, 0.7f, 0.7f);
			scena[i]->GetShader()->setVec3("light.specular", 1.0f, 1.0f, 1.0f);
			scena[i]->GetShader()->setVec3("light.direction", -0.2f, -1.0f, 10.0f);
			if (night)
			{
				scena[i]->GetShader()->setVec3("light.position", lightPos);
				scena[i]->GetShader()->setFloat("light.constant", 1.0f);
				scena[i]->GetShader()->setFloat("light.linear", 0.09f);
				scena[i]->GetShader()->setFloat("light.quadratic", 0.032f);

				scena[i]->GetShader()->setVec3("light.position", cameras[currentCamera]->position);
				scena[i]->GetShader()->setVec3("light.direction", cameras[currentCamera]->front);
				scena[i]->GetShader()->setFloat("light.cutOff", glm::cos(glm::radians(12.5f)));
				scena[i]->GetShader()->setFloat("light.outerCutOff", glm::cos(glm::radians(17.5f)));
			}
			glm::mat4 view = cameras[currentCamera]->GetViewMatrix();
			glm::mat4 projection;
			projection = glm::perspective(glm::radians(cameras[currentCamera]->zoom), screenWidth / screenHeight, 0.1f, 100.0f);

			//set uniform variables
			int viewLoc = glGetUniformLocation(scena[i]->GetShader()->id, "view");
			glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
			int projectionLoc = glGetUniformLocation(scena[i]->GetShader()->id, "projection");
			glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));
		}
	}
#pragma endregion

#pragma region Objects Setup
	for (int i = 0; i < scena.size(); ++i)
		if (scena[i]->IsActive())
			scena[i]->Draw();
#pragma endregion
}

#pragma endregion

#pragma region Dealocare
void deleteAll()
{
	BufferManager::DeleteBuffers();
	delete directionalShader;
	delete flashShader;
	delete raycastShader;
	for (int i = 0; i < 3; ++i)
		delete cameras[i];
	scena.clear();
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
		update();
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
