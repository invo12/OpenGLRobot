#pragma once
#include <vector>
#include <string>
#include<map>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/matrix4x4.h>
#include <assimp/cimport.h>
#include <iostream>
#include <glm/glm.hpp>
using namespace std;

struct Material {
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;
	float specularExponent;
	float dissolved;
	float density;
};

struct TextureInfo {
	unsigned int texID = 0;
	unsigned int specTexID = 0;
	unsigned int normalTexID;
};
class ObjectLoader
{ 
private:
	static map<string,vector<float>> vertexMap;
	static map<string, vector<unsigned int>> indexMap;
	static map<string, Material> materialMap;
public:
	static vector<float>* GetVertexBuffer(string name);
	static vector<unsigned int>* GetIndexBuffer(string name);
	static Material GetMaterial(string name);
};

