#include "Texture.h"

Texture::Texture()
{
    this->textureData = nullptr;
    this->width = 0;
    this->height = 0;
    this->nrChannels = 0;
}

Texture::Texture(std::string path)
{
    std::string textPath = path + "_diffuse.jpg";
    this->textureData = stbi_load(textPath.c_str(), &this->width, &this->height, &this->nrChannels, 0);
}

Texture::~Texture()
{
    delete[] textureData;
}
