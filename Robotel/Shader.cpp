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
	const char* vertexShaderSource = loadShaderFromFile(vertexPath);
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
	const char* fragmentShaderSource = loadShaderFromFile(fragmentPath);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);

	//nu uita sa dealoci
	delete[] fragmentShaderSource;

	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	id = glCreateProgram();
	glAttachShader(id, vertexShader);
	glAttachShader(id, fragmentShader);
	glLinkProgram(id);

	glGetProgramiv(id, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(id, 512, NULL, infoLog);
		cout << "EROARE LA LINK" << infoLog<< endl;
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
void Shader::setVec2(const std::string& name, const glm::vec2& value) const
{
	glUniform2fv(glGetUniformLocation(id, name.c_str()), 1, &value[0]);
}
void Shader::setVec2(const std::string& name, float x, float y) const
{
	glUniform2f(glGetUniformLocation(id, name.c_str()), x, y);
}
// ------------------------------------------------------------------------
void Shader::setVec3(const std::string& name, const glm::vec3& value) const
{
	glUniform3fv(glGetUniformLocation(id, name.c_str()), 1, &value[0]);
}
void Shader::setVec3(const std::string& name, float x, float y, float z) const
{
	glUniform3f(glGetUniformLocation(id, name.c_str()), x, y, z);
}
// ------------------------------------------------------------------------
void Shader::setVec4(const std::string& name, const glm::vec4& value) const
{
	glUniform4fv(glGetUniformLocation(id, name.c_str()), 1, &value[0]);
}
void Shader::setVec4(const std::string& name, float x, float y, float z, float w)
{
	glUniform4f(glGetUniformLocation(id, name.c_str()), x, y, z, w);
}
// ------------------------------------------------------------------------
void Shader::setMat2(const std::string& name, const glm::mat2& mat) const
{
	glUniformMatrix2fv(glGetUniformLocation(id, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}
// ------------------------------------------------------------------------
void Shader::setMat3(const std::string& name, const glm::mat3& mat) const
{
	glUniformMatrix3fv(glGetUniformLocation(id, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}
// ------------------------------------------------------------------------
void Shader::setMat4(const std::string& name, const glm::mat4& mat) const
{
	glUniformMatrix4fv(glGetUniformLocation(id, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}