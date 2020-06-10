#pragma once
#include "stb_image.h"
#include <string>
class Texture
{
private:
	unsigned char* textureData;
	int width, height, nrChannels;
public:
	Texture();
	Texture(std::string path);
	~Texture();
};

