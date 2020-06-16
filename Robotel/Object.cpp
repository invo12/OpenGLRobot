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
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);

		////normale
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);

		//texturi
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)(6 * sizeof(float)));
		glEnableVertexAttribArray(2);

		//tangente
		glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)(8 * sizeof(float)));
		glEnableVertexAttribArray(3);

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

	//initialize box collider ONLY AFTER VERTEX BUFFER
	this->collider = new BoxCollider(ObjectLoader::GetColliderMin(name), ObjectLoader::GetColliderMax(name));

	//initialize VAO, VBO, EBO
	initBuffers();

	//set materials and textures
	material = ObjectLoader::GetMaterial(numeFisier);
	this->textureInfo = TextureInfo{0,0,0};
	this->textureInfo.texID = TextureManager::GetTextureID(name);
    this->textureInfo.specTexID = TextureManager::GetTextureID(name + "_specular");
	this->textureInfo.normalTexID = TextureManager::GetTextureID(name + "_normal");

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
		SetScale(scale);
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
	calcModel();
}

void Object::Rotate(float degAngle, Axis rotationAxis)
{
	if (rotationAxis == Axis::x)
	{
		rotation.x += degAngle;
	}
	else if (rotationAxis == Axis::y)
	{
		rotation.y += degAngle;
	}
	else
	{
		rotation.z += degAngle;
	}
	calcModel();
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
	calcModel();
}

void Object::calcModel()
{
	this->model = glm::mat4(1.0f);
	this->model = glm::translate(this->model,position);
	
	this->model = glm::rotate(this->model, glm::radians(rotation.x), glm::vec3(1, 0, 0));
	this->model = glm::rotate(this->model, glm::radians(rotation.y), glm::vec3(0, 1, 0));
	this->model = glm::rotate(this->model, glm::radians(rotation.z), glm::vec3(0, 0, 1));
	
	this->model = glm::scale(this->model, scale);

	normal = glm::mat3(glm::transpose(glm::inverse(model)));
	glm::vec4 a = model * glm::vec4(this->collider->GetMax(), 1);
	glm::vec4 b = model * glm::vec4(this->collider->GetMin(), 1);
	
	//RESET THE COLLIDER
	this->collider = new BoxCollider(ObjectLoader::GetColliderMin(name), ObjectLoader::GetColliderMax(name));
	glm::vec3 mini = this->collider->GetMin();
	glm::vec3 maxi = this->collider->GetMax();
	vector<glm::vec3> corners;
	corners.push_back(mini);
	corners.push_back(maxi);
	corners.push_back(glm::vec3(mini.x, mini.y, maxi.z));
	corners.push_back(glm::vec3(mini.x, maxi.y, maxi.z));
	corners.push_back(glm::vec3(mini.x, maxi.y, mini.z));
	corners.push_back(glm::vec3(maxi.x, mini.y, mini.z));
	corners.push_back(glm::vec3(maxi.x, mini.y, maxi.z));
	corners.push_back(glm::vec3(maxi.x, maxi.y, mini.z));
	for (int i = 0; i < corners.size();++i)
		corners[i] = glm::vec3(model * glm::vec4(corners[i], 1));
	this->collider->SetBounds(corners);

	/*std::cout << position.x << ' ' << position.y << ' ' << position.z << endl;
	std::cout << rotation.x << ' ' << rotation.y << ' ' << rotation.z << endl;
	std::cout << scale.x << ' ' << scale.y << ' ' << scale.z << endl << endl << endl;*/
}
#pragma endregion

glm::vec3 Object::GetPosition()
{
	return position;
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

BoxCollider* Object::GetCollider()
{
	return collider;
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

	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, textureInfo.normalTexID);
	shader->setInt("normalMap", 2);
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
	delete collider;
}

std::vector<float> Object::GetVertexBuffer()
{
    return *this->vertexBuff;
}

std::vector<unsigned int> Object::GetIndexBuffer()
{
    return *this->indexBuff;
}

