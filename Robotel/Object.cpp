#define _CRT_SECURE_NO_WARNINGS
#include "Object.h"

void Object::initBuffers()
{
	if (BufferManager::GetVAO(name) == BUFFER_NOT_FOUND)
	{
		unsigned int VBO, EBO;
		//genereaza bufferele necesare
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);
		glGenBuffers(1, &EBO);

		//pentru obiecte
		glBindVertexArray(VAO);

		//pune in buffer informatiile
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, (*vertexBuff).size() * sizeof(float), &(*vertexBuff)[0], GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, (*indexBuff).size() * sizeof(unsigned int), &(*indexBuff)[0], GL_STATIC_DRAW);

		//seteaza atributele
		//pozitia
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);

		//normale
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);

		//texturi
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
		glEnableVertexAttribArray(2);

		//dai unbind
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);

		//setezi in buffer manager
		BufferManager::SetBuffers(name, VAO, VBO, EBO);
	}
	else
	{
		VAO = BufferManager::GetVAO(name);
	}
}

Object::Object(std::string numeFisier, Shader* shader)
{
	name = numeFisier;
	VAO = 0;
	
	vertexBuff = ObjectLoader::GetVertexBuffer(numeFisier);
	indexBuff = ObjectLoader::GetIndexBuffer(numeFisier);

	initBuffers();
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

