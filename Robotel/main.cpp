#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <string>
#include <fstream>

#include "Shader.h"
#include "Camera.h"
#include "stb_image.h"
using namespace std;

float vertices[] = {
	-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
	 0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
	-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

	-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
};
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

Camera* mainCamera;

float screenWidth = 800, screenHeight = 600;

//for speed normalization
float deltaTime = 0.0f;	// Time between current frame and last frame
float lastFrame = 0.0f; // Time of last frame

//for rotation of camera
float lastX = 400, lastY = 300;
bool firstMouse = true;

unsigned int VBO, VAO;
Shader* basicShader;
unsigned int grassTexture, brickTexture;

bool wireframe = false;
bool grass = true;
float angle = 0;

#pragma region Initializari

void initBuffers() 
{	
	//genereaza bufferele necesare
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	//bind vertex array
	glBindVertexArray(VAO);

	//pune in buffer informatiile
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	//seteaza atributele
	//pozitia
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	//culoarea
	/*glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3* sizeof(float)));
	glEnableVertexAttribArray(1);
*/
	//textura
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(2);

	//dai unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void initTextures()
{
	int width, height, nrChannels;
	unsigned char* data = stbi_load("grassTexture.jpg", &width, &height, &nrChannels, 0);

	glGenTextures(1, &grassTexture);

	//setari 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glBindTexture(GL_TEXTURE_2D, grassTexture);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else {
		cout << "Nu am putut incarca textura";
	}
	stbi_image_free(data);

	//unbind
	glBindTexture(GL_TEXTURE_2D, 0);
	
	//brick texture
	glGenTextures(1, &brickTexture);


	glBindTexture(GL_TEXTURE_2D, brickTexture);
	data = stbi_load("brickTexture.jpg", &width, &height, &nrChannels, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else {
		cout << "Nu am putut incarca textura";
	}
	stbi_image_free(data);
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
	if (key == GLFW_KEY_2 && action == GLFW_PRESS)
	{
		swap(grassTexture, brickTexture);
	}
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
	//program
	basicShader->use();
	
	//texturare
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, grassTexture);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, brickTexture);

	basicShader->setInt("texture2", 1);
	
	
	//transformari
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::rotate(model, (float)glfwGetTime() * glm::radians(50.0f), glm::vec3(0.5f, 1.0f, 0.0f));
	
	//Matematica, keep for refrence
	////position of the camera
	//glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);

	////directia camerei
	//glm::vec3 cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);
	//glm::vec3 cameraDirection = glm::normalize(cameraPos - cameraTarget);

	////dreapta camerei
	//glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
	//glm::vec3 cameraRight = glm::normalize(glm::cross(up, cameraDirection));

	////axa verticala a camerei
	//glm::vec3 cameraUp = glm::cross(cameraDirection, cameraRight);

	//ce ai facut mai sus poti face prin lookAt
	glm::mat4 view = mainCamera->GetViewMatrix();

	glm::mat4 projection;
	projection = glm::perspective(glm::radians(mainCamera->zoom), screenWidth / screenHeight, 0.1f, 100.0f);


	//set uniform variables
	int modelLoc = glGetUniformLocation(basicShader->id, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
	int viewLoc = glGetUniformLocation(basicShader->id, "view");
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
	int projectionLoc = glGetUniformLocation(basicShader->id, "projection");
	glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

	glBindVertexArray(VAO);
	for (int i = 0; i < 10; ++i)
	{
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, cubePositions[i]);
		float angle = 20.0f * (i+1);
		model = glm::rotate(model, (float)glfwGetTime() * glm::radians(angle) * (i%2 == 0 ? 1 : -1), glm::vec3(1.0f, 0.3f, 0.5f));
		basicShader->setMat4("model", model);
		glDrawArrays(GL_TRIANGLES, 0, 36);
	}


	glDrawArrays(GL_TRIANGLES, 0, 36);
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

	///////////////////////////////////////
	//AICI INITIALIZAM ARRAY-UL DE VERTICES
	///////////////////////////////////////
	//initializeVertexArrayFromFile("triunghi.txt");
	
	///////////////////////////////////////
	//AICI INITIALIZAM BUFFERELE///////////
	///////////////////////////////////////
	initBuffers();
	initTextures();
	basicShader = new Shader("vertex.vert","fragment.frag");
	mainCamera = new Camera();
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
	///////////////////////////////////////
	//AICI DEALOCAM ARRAY-UL DE VERTICES
	///////////////////////////////////////


	//dealocate resources
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	delete basicShader;
	delete mainCamera;
	glfwTerminate();
	return 0;

}
