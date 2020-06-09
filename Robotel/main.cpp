#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <string>
#include <fstream>

#include "Shader.h"
#include "stb_image.h"
using namespace std;

float vertices[] = {
	// positions         // colors
	 0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f, 1.0f, 0.0f,   // bottom right
	-0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f, 0.0f, 0.0f,   // bottom left
	 0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f, 0.5f, 1.0f    // top 
};
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
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	//culoarea
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3* sizeof(float)));
	glEnableVertexAttribArray(1);

	//textura
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
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
void framebuffer_size_callback(GLFWwindow* window, int width, int height);

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
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
void processInput(GLFWwindow* window)
{
	//if the user presses ESC then close the app
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
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
	glm::mat4 trans = glm::mat4(1.0f);
	trans = glm::rotate(trans, glm::radians(angle), glm::vec3(0.0, 0.0, 1.0));
	trans = glm::scale(trans, glm::vec3(2, 2, 2));
	unsigned int transformLoc = glGetUniformLocation(basicShader->id, "transform");
	glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(trans));
	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, 3);
}

#pragma endregion


int main()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	GLFWwindow* window = glfwCreateWindow(800, 600, "Robotel", NULL, NULL);
	if (window == NULL)
	{
		cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	//callback on resize
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glewInit();

	glfwSetKeyCallback(window, key_callback);
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
	//keep the window open
	while (!glfwWindowShouldClose(window))
	{
		//input
		processInput(window);
		
		//rendering
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
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
	glfwTerminate();
	return 0;

}
