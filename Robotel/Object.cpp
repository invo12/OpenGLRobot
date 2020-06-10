#define _CRT_SECURE_NO_WARNINGS
#include "Object.h"


Object::Object(std::string numeFisier)
{
    Assimp::Importer importer;
    aiMesh* mesh;
    std::string importModelPath = numeFisier + ".obj";
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
        }
        for (unsigned int f = 0; f < mesh->mNumFaces; f++)
        {
            face = &mesh->mFaces[f];
            indexBuff.push_back(face->mIndices[0]);
            indexBuff.push_back(face->mIndices[1]);
            indexBuff.push_back(face->mIndices[2]);
        }
        
    }

    std::string importMaterialPath = numeFisier + ".mtl";
    FILE* file = fopen(importMaterialPath.c_str(), "r");
    if (file == NULL) {
        printf("Impossible to open material!\n");
    }
    else {
        while (1) {
            char lineHeader[128];
            int res = fscanf(file, "%s", lineHeader);
            if (res == EOF) { break; }
            if (strcmp(lineHeader, "Ka") == 0) {
                glm::vec3 ambient;
                fscanf(file, "%f %f %f\n", &ambient.x, &ambient.y, &ambient.z);
                this->materialData.ambient = ambient;
            }
            else if (strcmp(lineHeader, "Kd") == 0) {
                glm::vec3 diffuse;
                fscanf(file, "%f %f %f\n", &diffuse.x, &diffuse.y, &diffuse.z);
                this->materialData.diffuse = diffuse;
            } if (strcmp(lineHeader, "Ks") == 0) {
                glm::vec3 specular;
                fscanf(file, "%f %f %f\n", &specular.x, &specular.y, &specular.z);
                this->materialData.specular = specular;
            } if (strcmp(lineHeader, "Ns") == 0) {
                float exponent;
                fscanf(file, "%f\n", &exponent);
                this->materialData.specularExponent = exponent;
            }if (strcmp(lineHeader, "d") == 0) {
                float dissolved;
                fscanf(file, "%f\n", &dissolved);
                this->materialData.dissolved = dissolved;
            }if (strcmp(lineHeader, "Ni") == 0) {
                float density;
                fscanf(file, "%f\n", &density);
                this->materialData.density = density;
            }
        }
    }
    this->objectTexture =new Texture(numeFisier);
}

void Object::ResetModelMatrix()
{
    this->model = glm::mat4(1.0f);
}

glm::mat4 Object::GetModelMatrix()
{
    return this->model;
}

void Object::Translate(glm::vec3 translate)
{
    this->model = glm::translate(this->model, glm::vec3(translate.x, translate.y, translate.z));
}

void Object::Rotate(float angle1, glm::vec3 rotationAxis)
{
    this->model = glm::rotate(this->model, angle1, rotationAxis);
}

void Object::Scale(glm::vec3 scale)
{
    this->model = glm::scale(this->model, scale);
}

Object::Object(const Object& object)
{
    this->vertexBuff = object.vertexBuff;
    this->indexBuff = object.indexBuff;
    this->model = object.model;
}

Texture* Object::GetTexture()
{
    return objectTexture;
}

void Object::ChangeTexture(std::string path)
{
    if (objectTexture != nullptr) {
        delete objectTexture;
        objectTexture = nullptr;
    }
    objectTexture =new Texture(path);
}

Object::~Object()
{
    delete objectTexture;
}


std::vector<float> Object::GetVertexBuffer()
{
    return this->vertexBuff;
}

std::vector<unsigned int> Object::GetIndexBuffer()
{
    return this->indexBuff;

}
