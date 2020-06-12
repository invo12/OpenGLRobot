#define _CRT_SECURE_NO_WARNINGS
#include "Object.h"

Object::Object(std::string numeFisier, Shader* shader)
{
	name = numeFisier;
	VAO = 0;
	vertexBuff = ObjectLoader::GetVertexBuffer(numeFisier);
	indexBuff = ObjectLoader::GetIndexBuffer(numeFisier);
	material = ObjectLoader::GetMaterial(numeFisier);
	this->textureInfo = TextureInfo{0,0,0};
	this->textureInfo.texID = TextureManager::GetTextureID(name);
    this->textureInfo.specTexID = TextureManager::GetTextureID(name + "_specular");
	this->textureInfo.normalTexID = 0;
	this->model = glm::mat4(1.0f);
	this->normal = glm::mat4(1.0f);
	this->shader = shader;
}
void Object::SetVAO(int VAO)
{
	this->VAO = VAO;
}
void Object::ResetModelMatrix()
{
    this->model = glm::mat4(1.0f);
}

glm::mat4 Object::GetModelMatrix()
{
    return this->model;
}

void Object::Translate(glm::vec3 translate)
{
    this->model = glm::translate(this->model, glm::vec3(translate.x, translate.y, translate.z));
}

void Object::Rotate(float angle1, glm::vec3 rotationAxis)
{
    this->model = glm::rotate(this->model, angle1, rotationAxis);
}

void Object::Scale(glm::vec3 scale)
{
    this->model = glm::scale(this->model, scale);
}

string Object::GetName()
{
	return name;
}

void Object::SetShader(Shader* shader)
{
    this->shader = shader;
}

Shader* Object::GetShader()
{
    return this->shader;
}

void Object::Draw()
{
	//folosire shader
	shader->use();
	//setare parametri shader
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureInfo.texID);
	shader->setInt("material.diffuse", 0);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, textureInfo.specTexID);
	shader->setInt("material.specular", 1);
	shader->setFloat("material.shininess", material.specularExponent);
	shader->setMat4("model", model);

	//bind la VAO
	glBindVertexArray(VAO);
	//apel functie desenare
	glDrawElements(GL_TRIANGLES, (*indexBuff).size(), GL_UNSIGNED_INT, (void*)0);
}



Object::Object(const Object& object)
{
    this->vertexBuff = object.vertexBuff;
    this->indexBuff = object.indexBuff;
    this->model = object.model;

    this->material.ambient = object.material.ambient;
    this->material.density = object.material.density;
    this->material.diffuse = object.material.diffuse;
    this->material.dissolved = object.material.dissolved;
    this->material.specular = object.material.specular;
    this->material.specularExponent = object.material.specularExponent;
    this->shader = object.shader;
    this->textureInfo = object.textureInfo;
}

Object::~Object()
{
 
}


std::vector<float> Object::GetVertexBuffer()
{
    return *this->vertexBuff;
}

std::vector<unsigned int> Object::GetIndexBuffer()
{
    return *this->indexBuff;

}

