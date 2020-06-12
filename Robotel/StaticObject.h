#pragma once
#include "Object.h"

//il setezi la inceput si asa ramane, nu mai recalculezi
class StaticObject : public Object 
{
public:
	StaticObject(std::string numeFisier, Shader* shader);
	void Translate(glm::vec3 translate) override;
	void Rotate(float angle1, glm::vec3 rotationAxis) override;
	void Scale(glm::vec3 scale) override;
	void Draw() override;
};