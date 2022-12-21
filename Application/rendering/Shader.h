#pragma once
#include <string>
#include <unordered_map>
#include <glm/gtc/type_ptr.hpp>
#include <glm/vec4.hpp>
#include <memory>

//TODO: Convert this to infer that shader referes to a compiled shaderprogram not individual vector and frag shaders
class Shader
{
public:
	~Shader();
	//This forces the user of this class to pass in a bool to check if didSucceed 
	//wheras a Create static method can return nullptr OR ask for input reference and return bool which is argueably batter

	//Shader(const std::string& filePath, GLenum shaderType, bool& didSucceed);
	static std::shared_ptr<Shader> Create( const std::string& vecShaderFilePath, const std::string& fragShaderFilePath);
	//this is type conversion function (in c# this would be reffered to with the implicit keyword)
	operator GLuint () const { return _progRef; }
	void SetUniformv4(const std::string& name, glm::vec4& val);
	void SetUniformi(const std::string& name, int val);
	void SetMat4(const std::string& name, const glm::mat4& mat);
private:
	static std::string GetSource(const std::string& filePath);
	static GLuint LoadShader(const std::string& source, GLenum shaderType);
	int GetUniformId(const std::string& name);
	std::unordered_map<std::string, int> _shaderUniforms;
	GLuint _progRef;
};


