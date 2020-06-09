#include "Object.h"
#include <fstream>
#include <iostream>

Object::Object(std::string numeFisier)
{
	this->hasUV = false;
	this->hasNormal = false;
	std::ifstream fisierObj;
	std::string linie;
	fisierObj.open(numeFisier.c_str());

	if (fisierObj.is_open()) {
		while (fisierObj) {
			getline(fisierObj, linie);
			if (linie.length() > 1) {
				const char* linieVector = linie.c_str();
				switch (linieVector[0]) {
				case 'v':
					if (linieVector[1] == ' ') {
						this->vertices.push_back(StringToVec3(linie));
						std::cout << "veritce-> " << linie << std::endl;
					}
					else if (linieVector[1] == 't') {
						this->uvs.push_back(StringToVec2(linie));
						this->hasUV = true;
						std::cout << "uvs-> " << linie << std::endl;
					}
					else if (linieVector[1] == 'n') {
						this->normals.push_back(StringToVec3(linie));
						this->hasNormal = true;
						std::cout << "normale-> " << linie << std::endl;
					}
					break;
				case 'f':
					std::cout << "faces-> " << linie << std::endl;
					//handle faces
					break;
				default:break;
				}
			}
		}
	}
	else
	{
		std::cout << "Nu am putut deschide fisierul " << numeFisier << std::endl;
		exit(0);
	}
}

std::vector<glm::vec3> Object::GetVertices()
{
	return this->vertices;
}

std::vector<glm::vec2> Object::GetUvs()
{
	return this->uvs;
}

std::vector<glm::vec3> Object::GetNormals()
{
	return this->normals;
}


glm::vec2 Object::StringToVec2(std::string linieFisier)
{
	unsigned int lungime = linieFisier.length();
	const char* linieDate = linieFisier.c_str();

	unsigned int vertIndexStart = 3;

	while (vertIndexStart < lungime)
	{
		if (linieDate[vertIndexStart] != ' ')
			break;
		vertIndexStart++;
	}

	unsigned int vertIndexEnd = NextChrIndex(vertIndexStart, linieDate, lungime, ' ');
	float x = atof(linieFisier.substr(vertIndexStart, vertIndexEnd - vertIndexStart).c_str());
	
	vertIndexStart = vertIndexEnd + 1;
	vertIndexEnd = NextChrIndex(vertIndexStart, linieDate, lungime, ' ');
	float y = atof(linieFisier.substr(vertIndexStart, vertIndexEnd - vertIndexStart).c_str());
	return glm::vec2(x, y);
}



glm::vec3 Object::StringToVec3(std::string linieFisier)
{
	unsigned int lungime = linieFisier.length();
	const char* linieDate = linieFisier.c_str();

	unsigned int vertIndexStart = 2;

	while (vertIndexStart < lungime)
	{
		if (linieDate[vertIndexStart] != ' ')
			break;
		vertIndexStart++;
	}

	unsigned int vertIndexEnd = NextChrIndex(vertIndexStart, linieDate, lungime, ' ');
	float x = atof(linieFisier.substr(vertIndexStart, vertIndexEnd - vertIndexStart).c_str());

	vertIndexStart = vertIndexEnd + 1;
	vertIndexEnd = NextChrIndex(vertIndexStart, linieDate, lungime, ' ');


	float y= atof(linieFisier.substr(vertIndexStart, vertIndexEnd - vertIndexStart).c_str());

	vertIndexStart = vertIndexEnd + 1;
	vertIndexEnd = NextChrIndex(vertIndexStart, linieDate, lungime, ' ');

	float z= atof(linieFisier.substr(vertIndexStart, vertIndexEnd - vertIndexStart).c_str());
	return glm::vec3(x, y, z);
}


unsigned int Object::NextChrIndex(unsigned int start, const char* str, unsigned int length, char compareData)
{
	unsigned int result = start;
	while (result < length)
	{
		result++;
		if (str[result] == compareData)
			break;
	}
	return result;
}
