#include "TextureManager.h"

map<string, unsigned int> TextureManager::textureMap;

void TextureManager::LoadTexture(string name)
{
	if (textureMap.find(name) == textureMap.end())
	{
		int width, height, nrChannels;
		unsigned int id;
		glGenTextures(1, &id);
		glBindTexture(GL_TEXTURE_2D, id);
		unsigned char* data = stbi_load(name.c_str(), &width, &height, &nrChannels, 0);
		if (data)
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
			glGenerateMipmap(GL_TEXTURE_2D);
		}
		else {
			cout << "Nu am putut incarca textura " + name;
		}
		stbi_image_free(data);

		//unbind
		glBindTexture(GL_TEXTURE_2D, 0);
		textureMap[name] = id;
	}
}

unsigned int TextureManager::GetTextureID(string name)
{
	name = name + ".jpg";
	if (textureMap.find(name) == textureMap.end())
	{
		LoadTexture(name);
	}
	return textureMap[name];
}
