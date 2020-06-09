#include "Shader.h"

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

Shader::Shader(const char* vertexPath, const char* fragmentPath) {
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

	id = glCreateProgram();
	glAttachShader(id, vertexShader);
	glAttachShader(id, fragmentShader);
	glLinkProgram(id);

	glGetProgramiv(id, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(id, 512, NULL, infoLog);
		cout << "EROARE LA LINK" << endl;
	}
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}
//Shader::~Shader()
//{
//	glDeleteProgram(id);
//}
void Shader::use()
{
	glUseProgram(id);
}

void Shader::setBool(const string& name, bool value) const
{
	glUniform1i(glGetUniformLocation(id, name.c_str()), (int)value);
}
void Shader::setInt(const string& name, int value) const
{
	glUniform1i(glGetUniformLocation(id, name.c_str()), value);
}
void Shader::setFloat(const string& name, float value) const
{
	glUniform1f(glGetUniformLocation(id, name.c_str()), value);
}