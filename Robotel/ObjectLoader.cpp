#define _CRT_SECURE_NO_WARNINGS
#include "ObjectLoader.h"

map<string, vector<float>> ObjectLoader::vertexMap;
map<string, vector<unsigned int>> ObjectLoader::indexMap;
map<string, Material> ObjectLoader::materialMap;

vector<float>* ObjectLoader::GetVertexBuffer(string name)
{
	name = name + ".obj";
	if (vertexMap.find(name) != vertexMap.end()) {
		return &vertexMap[name];
	}
	else {
		vector<float> vertexBuff;
		Assimp::Importer importer;
		aiMesh* mesh;
		std::string importModelPath = name;
		const aiScene* modelScene = importer.ReadFile(importModelPath, aiProcess_MakeLeftHanded | aiProcess_FlipWindingOrder | aiProcess_FlipUVs | aiProcess_PreTransformVertices |
			aiProcess_CalcTangentSpace |
			aiProcess_GenSmoothNormals |
			aiProcess_Triangulate |
			aiProcess_FixInfacingNormals |
			aiProcess_FindInvalidData |
			aiProcess_ValidateDataStructure | 0
		);
		if (modelScene)
		{
			mesh = modelScene->mMeshes[0];
			aiFace* face;
			for (unsigned int v = 0; v < mesh->mNumVertices; v++)
			{
				vertexBuff.push_back(mesh->mVertices[v].x);
				vertexBuff.push_back(mesh->mVertices[v].y);
				vertexBuff.push_back(mesh->mVertices[v].z);

				vertexBuff.push_back(mesh->mNormals[v].x);
				vertexBuff.push_back(mesh->mNormals[v].y);
				vertexBuff.push_back(mesh->mNormals[v].z);

				if (mesh->HasTextureCoords(0)) {
					vertexBuff.push_back(mesh->mTextureCoords[0][v].x);
					vertexBuff.push_back(mesh->mTextureCoords[0][v].y);
				}
				else
				{
					vertexBuff.push_back(0);
					vertexBuff.push_back(0);
				}
				vertexBuff.push_back(mesh->mTangents[v].x);
				vertexBuff.push_back(mesh->mTangents[v].y);
				vertexBuff.push_back(mesh->mTangents[v].z);
			}
			vertexMap[name] = vertexBuff;
			return &vertexMap[name];
		}
		else {
			cout << "EROARE VERTEX LOAD";
			return nullptr;
		}
	}
}
vector<unsigned int>* ObjectLoader::GetIndexBuffer(string name)
{
	name = name + ".obj";
	if (indexMap.find(name) != indexMap.end()) {
		return &indexMap[name];
	}
	else {
		vector<unsigned int> indexBuff;
		Assimp::Importer importer;
		aiMesh* mesh;
		std::string importModelPath = name;
		const aiScene* modelScene = importer.ReadFile(importModelPath, aiProcess_MakeLeftHanded | aiProcess_FlipWindingOrder | aiProcess_FlipUVs | aiProcess_PreTransformVertices |
			aiProcess_CalcTangentSpace |
			aiProcess_GenSmoothNormals |
			aiProcess_Triangulate |
			aiProcess_FixInfacingNormals |
			aiProcess_FindInvalidData |
			aiProcess_ValidateDataStructure | 0
		);
		if (modelScene)
		{
			mesh = modelScene->mMeshes[0];
			aiFace* face;
			for (unsigned int f = 0; f < mesh->mNumFaces; f++)
			{
				face = &mesh->mFaces[f];
				indexBuff.push_back(face->mIndices[0]);
				indexBuff.push_back(face->mIndices[1]);
				indexBuff.push_back(face->mIndices[2]);
			}
			indexMap[name] = indexBuff;
			return &indexMap[name];
		}
		else {
			cout << "EROARE INDEX LOAD";
			return nullptr;
		}
	}

}

Material ObjectLoader::GetMaterial(string name)
{
	name = name + ".mtl";
	if (materialMap.find(name) != materialMap.end()) {
		return materialMap[name];
	}
	else {
		std::string importMaterialPath = name;
		FILE* file = fopen(importMaterialPath.c_str(), "r");
		if (file == NULL) {
			printf("Impossible to open material!\n");
			return Material{ glm::vec3(0),glm::vec3(0),glm::vec3(0),0,0,0 };
		}
		else {
			Material material = Material{ glm::vec3(0),glm::vec3(0),glm::vec3(0),0,0,0 };
			while (1) {
				char lineHeader[128];
				int res = fscanf(file, "%s", lineHeader);
				if (res == EOF) { break; }
				if (strcmp(lineHeader, "Ka") == 0) {
					glm::vec3 ambient;
					fscanf(file, "%f %f %f\n", &ambient.x, &ambient.y, &ambient.z);
					material.ambient = ambient;
				}
				else if (strcmp(lineHeader, "Kd") == 0) {
					glm::vec3 diffuse;
					fscanf(file, "%f %f %f\n", &diffuse.x, &diffuse.y, &diffuse.z);
					material.diffuse = diffuse;


				} if (strcmp(lineHeader, "Ks") == 0) {
					glm::vec3 specular;
					fscanf(file, "%f %f %f\n", &specular.x, &specular.y, &specular.z);
					material.specular = specular;
				} if (strcmp(lineHeader, "Ns") == 0) {
					float exponent;
					fscanf(file, "%f\n", &exponent);
					material.specularExponent = exponent;
				}if (strcmp(lineHeader, "d") == 0) {
					float dissolved;
					fscanf(file, "%f\n", &dissolved);
					material.dissolved = dissolved;
				}if (strcmp(lineHeader, "Ni") == 0) {
					float density;
					fscanf(file, "%f\n", &density);
					material.density = density;
				}
			}
			materialMap[name] = material;
			return materialMap[name];
		}
	}
}
