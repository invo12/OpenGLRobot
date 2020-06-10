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
#include "Texture.h"
struct Material {
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;
	float specularExponent;
	float dissolved;
	float density;
};
class Object
{
private:
	Material materialData;
	glm::mat4 model = glm::mat4(1.0f);
	std::vector<float> vertexBuff;
	std::vector<unsigned int>indexBuff;
	Texture* objectTexture;
public:
	Object(std::string numeFisier);
	Object(const Object& object);
	~Object();

	std::vector<float> GetVertexBuffer();
	std::vector<unsigned int>GetIndexBuffer();

	void ResetModelMatrix();
	glm::mat4 GetModelMatrix();

	void Translate(glm::vec3 translate);
	void Rotate(float angle1,glm::vec3 rotationAxis);
	void Scale(glm::vec3 scale);
	
	Texture* GetTexture();
	void ChangeTexture(std::string path);


	//TODO CLONE!
	//TODO ADAUGARE SHADER IN OBIECT
	//Fisier Normala
	//Draw()

};







