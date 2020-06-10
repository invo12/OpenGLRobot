#define _CRT_SECURE_NO_WARNINGS
#include "Object.h"
#include <fstream>
#include <iostream>

Object::Object(std::string numeFisier)
{
	std::vector< unsigned int > vertexIndices, uvIndices, normalIndices;
	std::vector< glm::vec3 > temp_vertices;
	std::vector< glm::vec2 > temp_uvs;
	std::vector< glm::vec3 > temp_normals;
	this->hasUV = false;
	this->hasNormal = false;

	FILE* file = fopen(numeFisier.c_str(), "r");
	if (file == NULL) {
		printf("Impossible to open the file !\n");
		exit(0);
	}
	while (1) {
		char lineHeader[256];
		int res = fscanf(file, "%s", lineHeader);
		if (res == EOF)
		{
			break;
		}
		if (strcmp(lineHeader, "v") == 0) {
			glm::vec3 vertex;
			fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z);
			temp_vertices.push_back(vertex);
		}
		else if (strcmp(lineHeader, "vt") == 0) {
			glm::vec2 uv;
			fscanf(file, "%f %f\n", &uv.x, &uv.y);
			temp_uvs.push_back(uv);
		}
		else if (strcmp(lineHeader, "vn") == 0) {
			glm::vec3 normal;
			fscanf(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z);
			temp_normals.push_back(normal);
		}
		else if (strcmp(lineHeader, "f") == 0) {
			std::string vertex1, vertex2, vertex3;
			unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
			int matches = fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &vertexIndex[0], &uvIndex[0], &normalIndex[0], &vertexIndex[1], &uvIndex[1], &normalIndex[1], &vertexIndex[2], &uvIndex[2], &normalIndex[2]);
			if (matches != 9) {
				printf("File can't be read by our simple parser : ( Try exporting with other options\n");
				exit(0);
			}
			vertexIndices.push_back(vertexIndex[0]);
			vertexIndices.push_back(vertexIndex[1]);
			vertexIndices.push_back(vertexIndex[2]);
			uvIndices.push_back(uvIndex[0]);
			uvIndices.push_back(uvIndex[1]);
			uvIndices.push_back(uvIndex[2]);
			normalIndices.push_back(normalIndex[0]);
			normalIndices.push_back(normalIndex[1]);
			normalIndices.push_back(normalIndex[2]);
		}
	}
	for (unsigned int i = 0; i < vertexIndices.size(); i++) {
		unsigned int vertexIndex = vertexIndices[i];
		glm::vec3 vertex = temp_vertices[vertexIndex - 1];
		this->vertices.push_back(vertex);
	}

	for (unsigned int i = 0; i < uvIndices.size(); i++) {
		unsigned int uvIndex = uvIndices[i];
		glm::vec2 uv = temp_uvs[uvIndex - 1];
		this->textureCoordinates.push_back(uv);
	}

	for (unsigned int i = 0; i < normalIndices.size(); i++) {
		unsigned int normalIndex = normalIndices[i];
		glm::vec3 normal = temp_normals[normalIndex - 1];
		this->normals.push_back(normal);
	}
}

std::vector<glm::vec3> Object::GetVertices()
{
	return this->vertices;
}

std::vector<glm::vec2> Object::GetTextureCoordinates()
{
	return this->textureCoordinates;
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
