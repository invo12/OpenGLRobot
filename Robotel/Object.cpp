#include "Object.h"


Object::Object(std::string numeFisier)
{
    Assimp::Importer importer;
    const aiScene* modelScene = importer.ReadFile(numeFisier, aiProcess_MakeLeftHanded | aiProcess_FlipWindingOrder | aiProcess_FlipUVs | aiProcess_PreTransformVertices |
        aiProcess_CalcTangentSpace |
        aiProcess_GenSmoothNormals |
        aiProcess_Triangulate |
        aiProcess_FixInfacingNormals |
        aiProcess_FindInvalidData |
        aiProcess_ValidateDataStructure | 0
        );
    if (modelScene)
    {
        this->modelMesh = modelScene->mMeshes[0];
        std::cout << "a";
    }
    assimpGetMeshData(this->modelMesh);
}

bool Object::assimpGetMeshData(const aiMesh* mesh)
{
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
    return true;
}


std::vector<float> Object::GetVertexBuffer()
{
    return this->vertexBuff;
}

std::vector<unsigned int> Object::GetIndexBuffer()
{
    return this->indexBuff;
}
