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

Object::Object(std::string numeFisier, Shader* shader, glm::vec3 position, glm::vec3 rotation, glm::vec3 scale)
{
	//set name
	name = numeFisier;
	
	//set shader
	this->shader = shader;

	//Load vertex and index buffers
	vertexBuff = ObjectLoader::GetVertexBuffer(numeFisier);
	indexBuff = ObjectLoader::GetIndexBuffer(numeFisier);

	//initialize VAO, VBO, EBO
	initBuffers();

	//set materials and textures
	material = ObjectLoader::GetMaterial(numeFisier);
	this->textureInfo = TextureInfo{0,0,0};
	this->textureInfo.texID = TextureManager::GetTextureID(name);
    this->textureInfo.specTexID = TextureManager::GetTextureID(name + "_specular");
	this->textureInfo.normalTexID = 0;

	//set matrices
	this->model = glm::mat4(1.0f);
	this->normal = glm::mat4(1.0f);

	this->position = position;
	this->rotation = rotation;
	this->scale = scale;

	if (position != glm::vec3(0))
	{
		SetPosition(position);
	}
	if (rotation != glm::vec3(0))
	{
		SetRotation(rotation);
	}
	if (scale != glm::vec3(0))
	{

	}
}
void Object::SetVAO(int VAO)
{
	this->VAO = VAO;
}

void Object::Reset()
{
	this->position = glm::vec3(0);
	this->rotation = glm::vec3(0);
	this->scale = glm::vec3(0);
    this->model = glm::mat4(1.0f);
}

glm::mat4 Object::GetModelMatrix()
{
    return this->model;
}


#pragma region Transformations

void Object::Translate(glm::vec3 translate)
{
	position += translate;
    this->model = glm::translate(this->model, glm::vec3(translate.x, translate.y, translate.z));
	normal = glm::mat3(glm::transpose(glm::inverse(model)));
}

void Object::Rotate(float degAngle, Axis rotationAxis)
{
	glm::vec3 axisVector(1,0,0);
	if (rotationAxis == Axis::x)
	{
		axisVector = glm::vec3(1, 0, 0);
		rotation.x += degAngle;
	}
	else if (rotationAxis == Axis::y)
	{
		axisVector = glm::vec3(0, 1, 0);
		rotation.y += degAngle;
	}
	else
	{
		axisVector = glm::vec3(0, 0, 1);
		rotation.z += degAngle;
	}
    this->model = glm::rotate(this->model, glm::radians(degAngle), axisVector);
	normal = glm::mat3(glm::transpose(glm::inverse(model)));
}

void Object::SetPosition(glm::vec3 position)
{
	Translate(position - this->position);
}

void Object::SetRotation(glm::vec3 rotation)
{
	Rotate(rotation.x - this->rotation.x, Axis::x);
	Rotate(rotation.y - this->rotation.x, Axis::y);
	Rotate(rotation.z - this->rotation.x, Axis::z);
}

void Object::SetScale(glm::vec3 scale)
{
	this->scale = scale;
    this->model = glm::scale(this->model, scale);
	normal = glm::mat3(glm::transpose(glm::inverse(model)));
}
#pragma endregion


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
	shader->setMat3("normalMatrix", normal);

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

