#include "../include/Shader.h"
#include <string>
#include <GL/glew.h>
#include <fstream>

/*
Reads a file and converts it to a string.
*/
std::string readFile(const std::string& fileName)
{
	std::ifstream file(fileName);
	std::string output;

	if (!file.is_open())
	{
		printf("Error opening file: %s\n", fileName.c_str());
		return "-1";
	}

	std::string line;
	while(std::getline(file, line))
	{
		output += line + "\n";
	}

	return output;
}

Shader::Shader(
	const std::string& vertexFilePath, 
	const std::string& fragmentFilePath
)
{
	vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

	std::string vertexShaderCode = readFile(vertexFilePath);
	std::string fragmentShaderCode = readFile(fragmentFilePath);

	if (vertexShaderCode == "-1" ||
			fragmentShaderCode == "-1")
	{
		printf("Error loading shaders, files not found.\n");
	}

	//error checking from shaders
	GLint result = GL_FALSE;
	int infoLogLength;

	//===========VERTEX SHADER================
	//gotta convert it to a c string
	const char* vertexShaderCodeCStr = vertexShaderCode.c_str();

	//compile vertex shader
	printf("Compiling shader : %s\n", vertexFilePath.c_str());
	glShaderSource(vertexShaderID, 1, &vertexShaderCodeCStr, NULL);
	glCompileShader(vertexShaderID);

		//checking vertex shader
	glGetShaderiv(vertexShaderID, GL_COMPILE_STATUS, &result);
	glGetShaderiv(vertexShaderID, GL_INFO_LOG_LENGTH, &infoLogLength);
	if (infoLogLength > 0)
	{
		std::string vertexShaderErrorMessage;
		vertexShaderErrorMessage.reserve(infoLogLength);

		glGetShaderInfoLog(vertexShaderID, infoLogLength, NULL, 
											&vertexShaderErrorMessage[0]);
		printf("%s\n", vertexShaderErrorMessage.c_str());
	}

	//=============FRAGMENT SHADER============
	const char* fragmentShaderCodeCStr = fragmentShaderCode.c_str();
	//compile fragment shader
	printf("Compiling shader : %s\n", fragmentFilePath.c_str());
	glShaderSource(fragmentShaderID, 1, &fragmentShaderCodeCStr, NULL);
	glCompileShader(fragmentShaderID);

	//checking fragment shader
	glGetShaderiv(fragmentShaderID, GL_COMPILE_STATUS, &result);
	glGetShaderiv(fragmentShaderID, GL_INFO_LOG_LENGTH, &infoLogLength);
	if (infoLogLength > 0)
	{
		std::string fragmentShaderErrorMessage;
		fragmentShaderErrorMessage.reserve(infoLogLength);

		glGetShaderInfoLog(fragmentShaderID, infoLogLength, NULL, &fragmentShaderErrorMessage[0]);
		printf("%s\n", fragmentShaderErrorMessage.c_str());
	}

	//LINKING PROGRAM=====================
	printf("Linking Program ...\n");
	programID = glCreateProgram();
	glAttachShader(programID, vertexShaderID);
	glAttachShader(programID, fragmentShaderID);
	glLinkProgram(programID);

	//check the program
	glGetProgramiv(programID, GL_LINK_STATUS, &result);
	glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &infoLogLength);
	if (infoLogLength > 0)
	{
		std::string programErrorMessage;
		programErrorMessage.reserve(infoLogLength);

		glGetProgramInfoLog(programID, infoLogLength, NULL, &programErrorMessage[0]);
		printf("%s\n", programErrorMessage.c_str());
	}
	else
	{
		printf("No errors\n");
	}

	//I don't know why these are here, but leave them
	glDetachShader(programID, vertexShaderID);
	glDetachShader(programID, fragmentShaderID);

	glDeleteShader(vertexShaderID);
	glDeleteShader(fragmentShaderID);
}

void Shader::bind()
{
	glUseProgram(programID);
}

//uniform creation
GLuint Shader::createUniform(const std::string& name)
{
	GLuint uniformID = glGetUniformLocation(programID, name.c_str());
	uniforms[name] = uniformID;
}

GLuint Shader::getUniformID(const std::string& name)
{
	return uniforms[name];
}

void Shader::setUniform(const std::string& name, const glm::mat4& matrix)
{
	glUniformMatrix4fv(uniforms[name], 1, GL_FALSE, &matrix[0][0]);
}

//incase you want to do something with the program
GLuint Shader::getProgramID() {return programID;}