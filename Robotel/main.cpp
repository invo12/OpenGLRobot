#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <string>
#include <fstream>
using namespace std;

float vertices[] = {
		-0.5f, -0.5f, 0.0f, // left  
		 0.5f, -0.5f, 0.0f, // right 
		 0.0f,  0.5f, 0.0f  // top   
};
unsigned int VBO, VAO;
unsigned int shaderProgram;

bool wireframe = false;

#pragma region Load din fisiere

//in document nu pui f sau ,
//void initializeVertexArrayFromFile(string path) {
//	long long m,n;
//	ifstream f(path);
//	
//	f >> m >> n;
//	vertices = new float[m*n];
//	for (int i = 0; i < m; ++i)
//	{
//		for (int j = 0; j < n; ++j) 
//		{
//			f >> vertices[i * n + j];
//		}
//	}
//	f.close();
//}

char* loadShaderFromFile(string path)
{
	string text;
	string line;
	ifstream f(path);
	while (getline(f, line)) {
		text += line + '\n';
	}
	f.close();
	char* tmp = new char[text.size() + 1];
	copy(text.begin(), text.end(), tmp);
	tmp[text.size()] = '\0';
	return tmp;
}
#pragma endregion

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
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	//dai unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void initShaders()
{
	//shaders
	unsigned int vertexShader;
	unsigned int fragmentShader;
	//error handling
	int  success;
	char infoLog[512];
	
	//initializeaza vertex shader
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	const char* vertexShaderSource = loadShaderFromFile("vertex.vert");
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);
	
	//nu uita sa dealoci
	delete[] vertexShaderSource;
	
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	//initializeaza fragment shader
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	const char* fragmentShaderSource = loadShaderFromFile("fragment.frag");
	cout << fragmentShaderSource;
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);

	//nu uita sa dealoci
	delete[] fragmentShaderSource;

	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		cout << "EROARE LA LINK" << endl;
	}
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}

#pragma endregion

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
	//schimba din normal in wireframe
	if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
	{
		if(!wireframe)
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		else
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		wireframe = !wireframe;
	}
}

#pragma endregion

#pragma region Desenare

void draw()
{
	glUseProgram(shaderProgram);
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

	///////////////////////////////////////
	//AICI INITIALIZAM ARRAY-UL DE VERTICES
	///////////////////////////////////////
	//initializeVertexArrayFromFile("triunghi.txt");
	
	///////////////////////////////////////
	//AICI INITIALIZAM BUFFERELE///////////
	///////////////////////////////////////
	initShaders();
	initBuffers();

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
	glDeleteProgram(shaderProgram);
	glfwTerminate();
	return 0;

}
