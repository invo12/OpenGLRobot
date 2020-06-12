#include "StaticObject.h"

StaticObject::StaticObject(std::string numeFisier, Shader* shader) : Object{ numeFisier, shader }
{}

void StaticObject::Translate(glm::vec3 translate)
{
	Object::Translate(translate);
	normal = glm::mat3(glm::transpose(glm::inverse(model)));
}

void StaticObject::Rotate(float angle1, glm::vec3 rotationAxis)
{
	Object::Rotate(angle1, rotationAxis);
	normal = glm::mat3(glm::transpose(glm::inverse(model)));
}

void StaticObject::Scale(glm::vec3 scale)
{
	Object::Scale(scale);
	normal = glm::mat3(glm::transpose(glm::inverse(model)));
}

void StaticObject::Draw()
{
	GetShader()->setMat4("model", model);
	GetShader()->setMat3("normalMatrix", normal);
	Object::Draw();
}
