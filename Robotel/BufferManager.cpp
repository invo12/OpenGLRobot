#include "BufferManager.h"

map<string, unsigned int> BufferManager::bufferMap;
vector<unsigned int> BufferManager::buffersVector;

unsigned int BufferManager::GetVAO(string name)
{
	if (bufferMap.find(name) != bufferMap.end())
	{
		return bufferMap[name];
	}
	else
	{
		return BUFFER_NOT_FOUND;
	}
}

void BufferManager::SetBuffers(string name, int VAO, int VBO, int EBO)
{
	bufferMap[name] = VAO;
	buffersVector.push_back(VBO);
	buffersVector.push_back(EBO);
}

void BufferManager::DeleteBuffers()
{
	for (auto it = bufferMap.begin(); it != bufferMap.end(); ++it)
	{
		glDeleteVertexArrays(1, &(*it).second);
	}
	for (int i = 0; i < buffersVector.size(); ++i)
	{
		glDeleteBuffers(1, &buffersVector[i]);
	}
}
