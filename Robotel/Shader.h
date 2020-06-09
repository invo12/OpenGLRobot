#pragma once
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <GL/glew.h>
using namespace std;

class Shader
{
public:
	//id-ul programului
	unsigned int id;

	//constructor
	Shader(const char* vertexPath, const char* fragmentPath);
	//~Shader();
	//use current program
	void use();

	//seteaza variabile uniforme
	void setBool(const string& name, bool value) const;
	void setInt(const string& name, int value) const;
	void setFloat(const string& name, float value) const;
};

