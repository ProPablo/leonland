
#include <glad/glad.h> 
#include <GLFW/glfw3.h>

#include "Shader.h"
#include "../Application.h"

#include <fstream>
#include <iostream>

//Look up batch rendering for this https://www.youtube.com/watch?v=v5UDqm3zvcw

bool Shader::Create(Shader& input, const std::string& vecShaderFilePath, const std::string& fragShaderFilePath)
{
    auto vecShader = LoadShader(GetSource(vecShaderFilePath), GL_VERTEX_SHADER);
    auto fragShader = LoadShader(GetSource(fragShaderFilePath), GL_FRAGMENT_SHADER);
    if (!vecShader || !fragShader)
        return false;

    input._progRef = glCreateProgram();
    glAttachShader(input._progRef, vecShader);
    glAttachShader(input._progRef, fragShader);
    glLinkProgram(input._progRef);

    glDeleteShader(vecShader);
    glDeleteShader(fragShader);

    int success;
    char infoLog[512];
    glGetProgramiv(input._progRef, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(input._progRef, 512, NULL, infoLog);
        log_error(infoLog);
        return false;
    }
    return true;
}

void Shader::SetUniformv4(const std::string& name, glm::vec4& val)
{
    //Vector version of glUniform4f, second arg: how many to send
    glUniform4fv(GetUniformId(name), 1, glm::value_ptr(val));
}
void Shader::SetUniformi(const std::string& name, int val)
{
    glUniform1i(GetUniformId(name), val);
}
void Shader::SetMat4(const std::string& name, const glm::mat4& mat)
{
    glUniformMatrix4fv(GetUniformId(name), 1, GL_FALSE, glm::value_ptr(mat));
}

int Shader::GetUniformId(const std::string& name)
{
    if (!_shaderUniforms[name])
        _shaderUniforms[name] = glGetUniformLocation(_progRef, name.c_str());

    return _shaderUniforms[name];
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

GLuint Shader::LoadShader(const std::string& source, GLenum shaderType)
{
    const char* shaderTypeString = (shaderType == GL_VERTEX_SHADER) ? "vertex" : "fragment";

    log_dbg("Shader type: " << shaderTypeString << " initialised");

    GLuint shaderRef = glCreateShader(shaderType);
    const GLchar* shaderSource = source.c_str();
    //arg 3 is meant to be an array of strings so char** strings
    //This is so you can provide multiple inputs of shaders at once
    glShaderSource(shaderRef, 1, &shaderSource, NULL);
    glCompileShader(shaderRef);
    int success;
    char infoLog[512];
    glGetShaderiv(shaderRef, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(shaderRef, 512, NULL, infoLog);
        log_error(infoLog);
        shaderRef = 0;
    }

    return shaderRef;
}
