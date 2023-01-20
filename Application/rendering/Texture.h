#pragma once
#include <glad/glad.h>
#include "../core/Log.h"
class Texture
{
public:
	//This ctor NEEDS to have a working set of data passed into it, the factory function is there to enforce what happens if nah
	Texture(unsigned char* data, const std::string& name,int width , int height);
	~Texture();
    static std::unique_ptr<Texture> GenerateTexture(const std::string& filePath);
	void Bind(uint32_t textureSlot);
	operator GLuint () const { return _rendererID; };

	std::string Name;
private:
	GLuint _rendererID;
};

