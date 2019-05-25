#include "../include/Mesh.h"

#include <iostream>
#include <fstream>
#include <sstream>

//helpful for parsing the file, thanks to my intro cs course for this function
void split(std::string temp, std::string words[], char del, int maxWords)
{
	std::stringstream tempStream(temp);
	int i = 0;
	std::string tempFromTemp;
	while(std::getline(tempStream, tempFromTemp, del))
	{
		if (i == maxWords)
			return;
		words[i] = tempFromTemp;
		i++;
	}
}

//supposed to read in obj files exporrted from blender
//with the following settings:
/*
include UV
triangulate faces
*/
	Mesh::Mesh(std::string fileName)
	{
		std::ifstream inFile(fileName);

		//if the file can't be opened we just turn it into a cube lol
		if(!inFile.is_open())
		{
			std::cerr << "ERROR: CANNOT OPEN MESH FILE: '" << fileName << "'!!!" << std::endl;
			std::cerr << "TURNING MESH INTO A CUBE!" << std::endl;
			turnIntoCube();
		}
	else
	{
		//the data in obj file is stored in a compact way
		//common vertices are one one line of the file, and then when 
		//it builds the faces of the mesh, it references the indeces of these 
		//common data points
		//these temp vectors are the common data points
		std::vector<GLfloat> tempVertices;
		std::vector<GLfloat> tempUVcoords;
		std::vector<GLfloat> tempNormals;

		std::string temp;
		while(std::getline(inFile, temp))
		{
			//add another delimeter to the end of the line
			temp += ' ';
			//skip comments in the file
			if (temp[0] == '#')
				continue;

			//if it is about to describe a vertex;
			else if(temp[0] == 'v')
			{
				//if its a texture coord
				if (temp[1] == 't')
				{
					std::string words[3];
					split(temp, words, ' ', 4);
					tempUVcoords.push_back(std::stof(words[1]));
					tempUVcoords.push_back(std::stof(words[2]));
				}
				//if it's a vertex normal
				else if (temp[1] == 'n')
				{
					std::string words[4];
					split(temp, words, ' ', 4);

					tempNormals.push_back(std::stof(words[1]));
					tempNormals.push_back(std::stof(words[2]));
					tempNormals.push_back(std::stof(words[3]));
				}
				//if its a vertex
				else
				{
					std::string words[4];
					split(temp, words, ' ', 4);

					tempVertices.push_back(std::stof(words[1]));
					tempVertices.push_back(std::stof(words[2]));
					tempVertices.push_back(std::stof(words[3]));
				}
			}

			//if it's a face
			else if(temp[0] == 'f')
			{
				std::string words[4];
				split(temp, words, ' ', 4);

				//face describes vertIndex/vertUv/normIndex in .obj file
				for(int i = 1; i < 4; i++)
				{
					std::string vertData[3];
					split(words[i], vertData, '/', 3);

					int vertIndex = std::stoi(vertData[0]);
					int vertUV = std::stoi(vertData[1]);
					int normIndex = std::stoi(vertData[2]);

					//have to push back 3 floats for each vertex index specified
					vertex_buffer_vector.push_back(tempVertices.at((vertIndex-1) * 3 + 0));
					vertex_buffer_vector.push_back(tempVertices.at((vertIndex-1) * 3 + 1));
					vertex_buffer_vector.push_back(tempVertices.at((vertIndex-1) * 3 + 2));

					//have to push back 2 floats for each uv coordinate specified
					texture_coord_buffer_vector.push_back(tempUVcoords.at((vertUV - 1) * 2 + 0));
					texture_coord_buffer_vector.push_back(tempUVcoords.at((vertUV - 1) * 2 + 1));

					//3 floats for a normal vector
					normal_buffer_vector.push_back(tempNormals.at((normIndex -1 ) * 3 + 0));
					normal_buffer_vector.push_back(tempNormals.at((normIndex -1 ) * 3 + 1));
					normal_buffer_vector.push_back(tempNormals.at((normIndex -1 ) * 3 + 2));
				}
			}

			//other stuff we don't care about from the file, -redundant but explicit
			else
			{
				continue;
			}
		}
	}//end else 

	//now we set the data to be loaded into our vaos and vbos
	vertex_buffer_data = &(vertex_buffer_vector[0]);
	texture_coord_buffer_data = &(texture_coord_buffer_vector[0]);
	normal_buffer_data = &(normal_buffer_vector[0]);

	//we must divide by three because there are 3 floats per vertex
	//this data member is important for the drawing and loading of the data
	this->numVertices = vertex_buffer_vector.size()/3;
	
	//then we load our vaos and vbos
	// loadVAOandVBOs();
}

//the default constructor just makes a cube...
Mesh::Mesh()
{
	turnIntoCube();
	// loadVAOandVBOs();
}

void Mesh::load()
{
	loadVAOandVBOs();
}

void Mesh::loadVAOandVBOs()
{
	//let's init our vao so we can store all of our data for this mesh
	glGenVertexArrays(1, &vertexArrayObjectID);
	glBindVertexArray(vertexArrayObjectID);

	//==============Vertex Buffer=============
	glGenBuffers(1,&vertexBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 3 * numVertices, 
							vertex_buffer_data, GL_STATIC_DRAW);

	// (ATTRIBUTE 0) will be the vertices position
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferID);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	//========================================

	//==========Texture Coord Buffer==========
	glGenBuffers(1, &textureCoordBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, textureCoordBufferID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float)*2*numVertices, 
							texture_coord_buffer_data, GL_STATIC_DRAW);

	// (ATTRIBUTE 1) will be the texture coordinates
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, textureCoordBufferID);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);
	//========================================

	// std::cout << "normal_buffer_vector.size(): " << normal_buffer_vector.size() << std::endl;
	// std::cout << "vertex_buffer_vector.size(): " << vertex_buffer_vector.size() << std::endl;

	//================NORMALS FROM FILE=================
	if (normal_buffer_data != nullptr)
	{
		glGenBuffers(1, &normalBufferID);
		glBindBuffer(GL_ARRAY_BUFFER, normalBufferID);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float)*3*numVertices,
			normal_buffer_data, GL_STATIC_DRAW);

		glEnableVertexAttribArray(2);
		glBindBuffer(GL_ARRAY_BUFFER, normalBufferID);
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);
	}
	//=================================================
}

//this is literally just hard coded data of a cube mesh 
//this is residue from the original testing parts of this program where I had 
//to input all the data manually.
void Mesh::turnIntoCube()
{
		vertex_buffer_data = new GLfloat[108] 
		{
	  //face close to camera
	   0.5f, 0.5f,-0.5f, //0
	  -0.5f, 0.5f,-0.5f, //1
	   0.5f,-0.5f,-0.5f, //2

	  0.5f,-0.5f,-0.5f, //2
	  -0.5f, 0.5f,-0.5f, //1
	  -0.5f,-0.5f,-0.5f, //3

	  //face far from camera
	   0.5f, 0.5f, 0.5f, //4
	  -0.5f, 0.5f, 0.5f, //5
	   0.5f,-0.5f, 0.5f, //6

	   0.5f,-0.5f, 0.5f, //6
	   -0.5f, 0.5f, 0.5f, //5
	   -0.5f,-0.5f, 0.5f, //7

	  //left face
	   -0.5f,-0.5f,-0.5f, //3
	   -0.5f, 0.5f,-0.5f, //1
	   -0.5f,-0.5f, 0.5f, //7

	   -0.5f,-0.5f, 0.5f, //7
	   -0.5f, 0.5f,-0.5f, //1
	   -0.5f, 0.5f, 0.5f, //5

	  //top face
	   -0.5f, 0.5f,-0.5f, //1
	   0.5f, 0.5f,-0.5f, //0
	   -0.5f, 0.5f, 0.5f, //5

	   -0.5f, 0.5f, 0.5f, //5
	   0.5f, 0.5f,-0.5f, //0
	   0.5f, 0.5f, 0.5f, //4

	   //right face
	   0.5f, 0.5f,-0.5f, //0
	   0.5f,-0.5f,-0.5f, //2
	   0.5f, 0.5f, 0.5f, //4

	   0.5f, 0.5f, 0.5f, //4
	   0.5f,-0.5f,-0.5f, //2
	   0.5f,-0.5f, 0.5f, //6

	   //bot face
	   0.5f,-0.5f,-0.5f, //2
	   -0.5f,-0.5f,-0.5f, //3
	   0.5f,-0.5f, 0.5f, //6

	   0.5f,-0.5f, 0.5f, //6
	   -0.5f,-0.5f,-0.5f, //3
	   -0.5f,-0.5f, 0.5f, //7
		};

		texture_coord_buffer_data = new GLfloat[72]
		{
	  1.0f, 1.0f,
	  0.0f, 1.0f, // face closest to camera
	  1.0f, 0.0f,

	  1.0f, 0.0f,
	  0.0f, 1.0f,
	  0.0f, 0.0f,
	  //the data is just copy/pasted from here on out
	  1.0f, 1.0f,
	  0.0f, 1.0f, 
	  1.0f, 0.0f,

	  1.0f, 0.0f,
	  0.0f, 1.0f,
	  0.0f, 0.0f,

	  1.0f, 1.0f,
	  0.0f, 1.0f, 
	  1.0f, 0.0f,

	  1.0f, 0.0f,
	  0.0f, 1.0f,
	  0.0f, 0.0f,

	  1.0f, 1.0f,
	  0.0f, 1.0f, 
	  1.0f, 0.0f,

	  1.0f, 0.0f,
	  0.0f, 1.0f,
	  0.0f, 0.0f,

	  1.0f, 1.0f,
	  0.0f, 1.0f, 
	  1.0f, 0.0f,

	  1.0f, 0.0f,
	  0.0f, 1.0f,
	  0.0f, 0.0f,

	  1.0f, 1.0f,
	  0.0f, 1.0f, 
	  1.0f, 0.0f,

	  1.0f, 0.0f,
	  0.0f, 1.0f,
	  0.0f, 0.0f,
	};

	this->numVertices = 36;
}

Mesh::~Mesh()
{
	//new was called in the constructor for these data pieces
	//might move these into std::vector containers later though.
	// delete vertex_buffer_data;
	// delete texture_coord_buffer_data;
}

void Mesh::bind() const
{
	//vao contains the buffers with all the data
	//neat little thing opengl thought of there
	glBindVertexArray(0);
	glBindVertexArray(vertexArrayObjectID);
}
void Mesh::draw() const
{
	//our data is stored as triangulated faces in format
	glDrawArrays(GL_TRIANGLES, 0, numVertices);
}