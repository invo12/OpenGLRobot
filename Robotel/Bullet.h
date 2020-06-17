#pragma once
#include "Object.h"
class Bullet :public Object
{
private:
	vector<Object*>* scena;
public:
	Bullet(Shader* shader, glm::vec3 position = glm::vec3(0), glm::vec3 rotation = glm::vec3(0), glm::vec3 scale = glm::vec3(1));
};

