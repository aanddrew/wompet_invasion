#ifndef TEXTURE_H
#define TEXTURE_H

#include <string>
#include <GL/glew.h>

class Texture
{
public:
	Texture();
	//the file to load must be a png with 24 bit (rgba) color depth.
	//I.E. you must add an alpha channel and export the image as a png
	Texture(std::string fileName);
	~Texture();

	//do this before drawing stuff that uses this texture
	void bind();
	void load();
private:
	//properties of the image we are loading
	int width;
	int height;
	int numChannels;
	//char so we can just get bytes of data in the system
	unsigned char * data;

	//handle to this texture in our opengl state
	GLuint textureID;
};

#endif