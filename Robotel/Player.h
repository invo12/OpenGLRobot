#pragma once
#include "Object.h"
class Player :public Object
{
private:
	vector<Object*>* scena;
public:
	Player(std::string numeFisier, Shader* shader, glm::vec3 position = glm::vec3(0), glm::vec3 rotation = glm::vec3(0), glm::vec3 scale = glm::vec3(1));
	void SetScene(vector<Object*>* scene);
	void Draw();
};

