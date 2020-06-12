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

class Object
{
private:
	Material material;
	std::vector<float>* vertexBuff;
	std::vector<unsigned int>* indexBuff;
	Shader* shader;
	TextureInfo textureInfo;
	string name;
	unsigned int VAO;

protected:
	glm::mat4 model;
	glm::mat4 normal;
	void initBuffers();

public:
	Object(std::string numeFisier,Shader* shader);
	~Object();

	std::vector<float> GetVertexBuffer();
	std::vector<unsigned int> GetIndexBuffer();

	void SetVAO(int VAO);
	void ResetModelMatrix();
	glm::mat4 GetModelMatrix();

	string GetName();
	void SetShader(Shader* shader);
	Shader* GetShader();

	virtual void Translate(glm::vec3 translate);
	virtual void Rotate(float angle1, glm::vec3 rotationAxis);
	virtual void Scale(glm::vec3 scale);
	virtual void Draw();
};







