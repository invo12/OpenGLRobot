#pragma once
#include <vector>
#include <string>
#include <glm/glm.hpp>


class Object
{
private:
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec2> uvs;
	std::vector<glm::vec3> normals;
	bool hasUV;
	bool hasNormal;

	glm::vec2 StringToVec2(std::string linieFisier);
	glm::vec3 StringToVec3(std::string linieFisier);
	unsigned int NextChrIndex(unsigned int start, const char* str, unsigned int length, char compareData);
public:
	Object(std::string numeFisier);
	std::vector<glm::vec3> GetVertices();
	std::vector<glm::vec2> GetUvs();
	std::vector<glm::vec3> GetNormals();
	//TODO Stack!
	//Object GetModel();
	//Translate
	//Rotate
	//Scale
	//Undo
	//bool hasUV();
	//bool hasNormal();
	//Draw

};







