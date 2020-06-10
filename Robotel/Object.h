#pragma once
#include <vector>
#include <string>
#include <glm/glm.hpp>
#include <fstream>
#include <iostream>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/matrix4x4.h>
#include <assimp/cimport.h>

class Object
{
private:
	const aiMesh* modelMesh;
	glm::mat4 model = glm::mat4(1.0f);

	std::vector<float> vertexBuff;
	std::vector<unsigned int>indexBuff;


public:
	Object(std::string numeFisier);
	std::vector<float> GetVertexBuffer();
	std::vector<unsigned int>GetIndexBuffer();
	bool assimpGetMeshData(const aiMesh* mesh);
	//model matrix
	//get model matrix
	//Object GetModel();
	//Translate
	//Rotate
	//Scale
	//Fisier Texturi
	//Fisier Normala
	//Draw

};







