#include "../include/Texture.h"
#include "../stbi/stb_image.h"

#include <iostream>
#include <GL/glew.h>

Texture::Texture()
{
	this->data = nullptr;
}

Texture::Texture(std::string fileName)
{
	this->data = stbi_load(fileName.c_str(), &width, &height, &numChannels, 0);

	if (data == nullptr)
	{
		std::cerr << "ERROR: CANNOT LOAD IMAGE FILE: " << fileName << "!!!" << std::endl;
	}
}

Texture::~Texture()
{
	//gotta free that data B)
	if (data != nullptr)
		stbi_image_free(data);
}

void Texture::bind()
{
	glActiveTexture(textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);
}

void Texture::load()
{
	//set up the texture to be used on the gpu
	glGenTextures(1, &(this->textureID));
	glActiveTexture(textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	//load it into the gpu
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
}