
#include <glad/glad.h> 
#include <GLFW/glfw3.h>
#include "Shader.h"
#include "../Application.h"
#include <fstream>
#include <iostream>

//Look up batch rendering for this https://www.youtube.com/watch?v=v5UDqm3zvcw

Shader::Shader(const std::string& filePath, GLenum shaderType, bool& didSucceed)
{
	didSucceed = true;
	const char* shaderTypeString = (shaderType == GL_VERTEX_SHADER) ? "vertex" : "fragment";

	log_dbg("Shader type: " << shaderTypeString << " initialised");

	_shaderRef = glCreateShader(shaderType);
	std::string source = GetSource(filePath);
	const GLchar* shaderSource = source.c_str();
	//arg 3 is meant to be an array of strings so char** strings
	//This is so you can provide multiple inputs of shaders at once
	glShaderSource(_shaderRef, 1, &shaderSource, NULL);
	glCompileShader(_shaderRef);
	int success;
	char infoLog[512];
	glGetShaderiv(_shaderRef, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(_shaderRef, 512, NULL, infoLog);
		log_error(infoLog);
		didSucceed = false;
	}
}

GLuint Shader::CompileProgram(const Shader& vert, const Shader& frag)
{
	GLuint res;
	res = glCreateProgram();
	glAttachShader(res, vert._shaderRef);
	glAttachShader(res, frag._shaderRef);
	glLinkProgram(res);

	int success;
	char infoLog[512];
	glGetProgramiv(res, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(res, 512, NULL, infoLog);
		log_error(infoLog);
	}
	return res;
}

//TODO: move this to helpers
//Copying is totally fine here, the string has to be half than max alloc available. If that is not the case, simply pass in mutable reference into this function
std::string Shader::GetSource(const std::string& filePath)
{
	std::ifstream inputStream(filePath, std::ios::in, std::ios::binary);
	std::string res;
	if (inputStream)
	{
		//seek to end, read in length
		inputStream.seekg(0, std::ios::end);
		res.resize(inputStream.tellg());
		//Go back to start, read into result
		inputStream.seekg(0, std::ios::beg);
		inputStream.read(&res[0], res.size());
		inputStream.close();
	}
	else
	{
		log_error(std::string("Could not read file") + filePath);
	}
	return res;
}
