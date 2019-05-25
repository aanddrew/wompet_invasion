#ifndef SHADER_H
#define SHADER_H

#include <string>
#include <unordered_map>

#include <GL/glew.h>
#include <glm/glm.hpp>

class Shader
{
public:
	Shader(const std::string& vertexFilePath, const std::string& fragmentFilePath);

	void bind();
	GLuint getProgramID();

	GLuint createUniform(const std::string& name);
	GLuint getUniformID(const std::string& name);
	void setUniform(const std::string& name, const glm::mat4& matrix);
private:
	GLuint vertexShaderID;
	GLuint fragmentShaderID;
	GLuint programID;

	std::unordered_map<std::string, GLuint> uniforms;
};

#endif