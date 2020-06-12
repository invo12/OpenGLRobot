#pragma once
#include <GL/glew.h>
#include <map>
#include <string>
#include <vector>
#define BUFFER_NOT_FOUND 1000000
using namespace std;

class BufferManager
{
private:
	//map from name to VAO
	static map<string, unsigned int> bufferMap;
	static vector<unsigned int> buffersVector;
public:
	static unsigned int GetVAO(string name);
	static void SetBuffers(string name, int VAO, int VBO, int EBO);
	static void DeleteBuffers();
};