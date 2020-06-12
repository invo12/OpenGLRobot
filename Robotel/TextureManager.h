#pragma once
#include <map>
#include <string>
#include <iostream>
#include <GL/glew.h>
#include "stb_image.h"
using namespace std;
class TextureManager
{
private:
	static map<string, unsigned> textureMap;
	static void LoadTexture(string name);
public:
	static unsigned int GetTextureID(string name);
};

