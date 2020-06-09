#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <string>
#include <fstream>
using namespace std;

float* vertices;
unsigned int VBO;

//in document nu pui f sau ,
void initializeVertexArray(string path) {
	long long m,n;
	ifstream f(path);
	
	f >> m >> n;
	vertices = new float[m*n];
	for (int i = 0; i < m; ++i)
	{
		for (int j = 0; j < n; ++j) 
		{
			f >> vertices[i * n + j];
		}
	}
	for (int i = 0; i < m; ++i)
	{
		for (int j = 0; j < n; ++j)
		{
			cout << vertices[i * n + j]<<' ';
		}
		cout << endl;
	}
}
void initBuffers() 
{
	glGenBuffers(1, &VBO);
}
#pragma region GLFW Input & Callbacks

//resize render area after you resize the window
void framebuffer_size_callback(GLFWwindow* window, int width, int height);

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window)
{
	//if the user presses ESC then close the app
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

#pragma endregion

int main()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	GLFWwindow* window = glfwCreateWindow(800, 600, "Robotel", NULL, NULL);
	if (window == NULL)
	{
		cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glViewport(0, 0, 800, 600);
	glewInit();

	//callback on resize
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	
	///////////////////////////////////////
	//AICI INITIALIZAM ARRAY-UL DE VERTICES
	///////////////////////////////////////
	initializeVertexArray("triunghi.txt");
	
	///////////////////////////////////////
	//AICI INITIALIZAM BUFFERELE///////////
	///////////////////////////////////////
	initBuffers();

	//keep the window open
	while (!glfwWindowShouldClose(window))
	{
		//input
		processInput(window);
		
		//rendering
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);


		//call events and swap buffers
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	///////////////////////////////////////
	//AICI DEALOCAM ARRAY-UL DE VERTICES
	///////////////////////////////////////
	delete[] vertices;

	//dealocate resources
	glfwTerminate();
	return 0;

}
