#pragma once
#include <glad/glad.h>
#include "../core/Log.h"
#include <glm/vec2.hpp>
class Texture
{
public:
	static const int DEFAULT_PIXELS_PER_UNIT = 100;
	//This ctor NEEDS to have a working set of data passed into it, the factory function is there to enforce what happens if nah
	Texture(unsigned char* data, const std::string& name,int width , int height);
	~Texture();
    static std::unique_ptr<Texture> GenerateTexture(const std::string& filePath);
	void Bind(uint32_t textureSlot);
	inline operator GLuint () const { return _rendererID; };
	// The inline's main purpose is to suppress the one definition rule so functions can be declared in headers but ALL IT DOES IS COPY PASTE CODE INTO WHEREVER ITS CALLED
	// Good optimization https://stackoverflow.com/questions/7113872/inline-vs-constexpr
	//Also the const also makes sure it not declared on heap (unchanging)
	inline bool operator ==(const Texture& rhs) const { _rendererID == rhs._rendererID; }

	std::string Name;
	glm::uvec2 size;
	//CHangeable after Create 
	int pixelsPerUnit = DEFAULT_PIXELS_PER_UNIT;
private:
	GLuint _rendererID;
};

