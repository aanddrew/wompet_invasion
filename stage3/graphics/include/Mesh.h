#ifndef MESH_H
#define MESH_H

#include <GL/glew.h>
#include <string>
#include <vector>

class Mesh
{
public:
	Mesh(std::string fileName);
	~Mesh();
	Mesh();

	void load();

	void bind() const;
	void draw() const;

	//make these private later
	//vertices

private:
	void turnIntoCube();
	void loadVAOandVBOs();

	//VAO of this mesh on the gpu
	GLuint vertexArrayObjectID;

	//where our data is stored in the gpu
	GLuint vertexBufferID;
	GLuint textureCoordBufferID;
	GLuint normalBufferID;

	//this is where our data is stored on ram
	GLfloat * vertex_buffer_data;
	GLfloat * texture_coord_buffer_data;
	GLfloat * normal_buffer_data;

	std::vector<GLfloat> vertex_buffer_vector;
	std::vector<GLfloat> texture_coord_buffer_vector;
	std::vector<GLfloat> normal_buffer_vector;

	unsigned int numVertices;
};
#endif