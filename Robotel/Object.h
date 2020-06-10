#pragma once
#include <vector>
#include <string>
#include <glm/glm.hpp>


class Object
{
private:
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec2> textureCoordinates;
	std::vector<glm::vec3> normals;
	bool hasUV;
	bool hasNormal;
	glm::mat4 model = glm::mat4(1.0f);

	glm::vec2 StringToVec2(std::string linieFisier);
	glm::vec3 StringToVec3(std::string linieFisier);
	unsigned int NextChrIndex(unsigned int start, const char* str, unsigned int length, char compareData);
public:
	Object(std::string numeFisier);
	std::vector<glm::vec3> GetVertices();
	std::vector< glm::vec2 > GetTextureCoordinates();
	std::vector<glm::vec3> GetNormals();
	//model matrix
	//get model matrix
	//Object GetModel();
	//Translate
	//Rotate
	//Scale
	//bool hasUV();
	//bool hasNormal();
	//Draw

};







