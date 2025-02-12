#pragma once
#include <vector>
#include <string>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <fstream>
#include <iostream>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/matrix4x4.h>
#include <assimp/cimport.h>

#include "Shader.h"
#include "ObjectLoader.h"
#include "TextureManager.h"
#include "BufferManager.h"
#include "BoxCollider.h"

enum class Axis {
	x,
	y,
	z
};
enum class Layer {
	Default,
	IgnoreRaycast
};
class Object
{
private:
	//buffers
	std::vector<float>* vertexBuff;
	std::vector<unsigned int>* indexBuff;
	unsigned int VAO, VAO1;
	
	//collider
	BoxCollider *collider;

	//shader
	Shader* shader;

	//textures
	TextureInfo textureInfo;
	
	//materials
	Material material;

	//info about object
	string name;
	glm::vec3 position, rotation, scale;
	
	//for vertex shader
	glm::mat4 model;
	glm::mat4 normal;

	//initialize VAO, VBO, EBO
	void initBuffers();
	void calcModel();

	Layer layer;
	bool active;
public:
	Object(std::string numeFisier, Shader* shader, glm::vec3 position = glm::vec3(0), glm::vec3 rotation = glm::vec3(0), glm::vec3 scale = glm::vec3(1));
	~Object();

	std::vector<float> GetVertexBuffer();
	std::vector<unsigned int> GetIndexBuffer();
	void SetVAO(int VAO);

	void SetShader(Shader* shader);
	Shader* GetShader();

	BoxCollider* GetCollider();
	void SetLayer(Layer l);
	Layer GetLayer();

	void SetActive(bool active);
	bool IsActive();
	//reset position, rotation and scale
	void Reset();
	glm::mat4 GetModelMatrix();

	string GetName();
	
	//move object around
	void Translate(glm::vec3 translate);
	void Rotate(float degAngle, Axis rotationAxis);
	
	glm::vec3 GetPosition();
	glm::vec3 GetRotation();

	//set object properties
	void SetPosition(glm::vec3 position);
	void SetRotation(glm::vec3 rotation);
	void SetScale(glm::vec3 scale);

	void Draw();
};