#pragma once
#include <string>
#include <map>
//Already declared in glad
//typedef char GLchar;
//typedef unsigned int GLuint;

//TODO: Convert this to infer that shader referes to a compiled shaderprogram not individual vector and frag shaders
//inludes:
//Upload uniforms (these are accessed fper program, not per shader)
//Error handling
class Shader
{
public:
	//This forces the user of this class to pass in a bool to check if didSucceed 
	//wheras a Create static method can return nullptr OR ask for input reference and return bool which is argueably batter
	Shader(const std::string& filePath, GLenum shaderType, bool& didSucceed);
	static GLuint CompileProgram(const Shader& vert, const Shader& frag);
	GLuint _shaderRef;
private:
	std::string GetSource(const std::string& filePath);
	GLuint LoadSourceShaderFromSource();
	std::map<std::string, int> shaderUniforms;
};


