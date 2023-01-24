#include "Texture.h"
#include "../stb/stb.h"
#include "../core/Log.h";
Texture::Texture(unsigned char* data, const std::string& name, int width, int height) : Name(name), size({width, height})
{

    //https://stackoverflow.com/questions/71284184/opengl-distorted-texture
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1); //This is how bitpacking works with unsinged bytes (255) instead of full floats, doesnt make things slow excpet texture loading
    //https://stackoverflow.com/questions/11042027/glpixelstoreigl-unpack-alignment-1-disadvantages
    //This suggests that when changing etxtures to and from gpu, try to make it multiple of 8


    glGenTextures(1, &_rendererID);
    glBindTexture(GL_TEXTURE_2D, _rendererID);

    //2nd arg: mipmap level
       //3rd arg: texture type
       //7.8th format and datatype of input
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
}

Texture::~Texture()
{
    glDeleteTextures(1, &_rendererID);
}

std::unique_ptr<Texture> Texture::GenerateTexture(const std::string& filePath)
{

    //OpenGl by default flips textures (uv starts at bottom left and the image is loaded from top left)
    stbi_set_flip_vertically_on_load(true);

    int width, height, nrChannels;
    unsigned char* data = stbi_load(filePath.c_str(), &width, &height, &nrChannels, 0);
    if (!data)
    {
        log_error("Failed to load texture " << filePath);
        return nullptr;
    }
    auto texture = std::make_unique<Texture>(data, filePath, width, height);

    stbi_image_free(data);

    return texture;
}

void Texture::Bind(uint32_t textureSlot)
{
    //So the ID is different from WHERE the texture is located within the slot array
    //There exists 32 slots on modern gpus, 8 on mobile

    // log_dbg("Binding texture " << _rendererID << "To texture slot " << textureSlot);
    //This method convention for loading a single texture into a slot 
    glActiveTexture(GL_TEXTURE0 + textureSlot); // define texture slot
    glBindTexture(GL_TEXTURE_2D, _rendererID);

    //Loading this texture into a slot directly
    //glBindTextureUnit(textureSlot, _rendererID);
}

