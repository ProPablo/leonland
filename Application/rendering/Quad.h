#pragma once
//This can be a single header file (needs to be used in multiple locations so cant be in single translation unit)
#include <glm/vec2.hpp>
#include <glm/mat4x4.hpp>
#include <glad/glad.h>
#include <array>
#include "Vertex.h"
#include "Texture.h"

class Quad
{
public:
    //Quad(glm::vec2 pos,glm::vec2 scale, float angle, GLuint textureID);
    Quad(glm::vec2 pos, glm::vec2 scale, float angle, Texture* tex, glm::vec4 color = {1,1,1,1});
    glm::vec2 Pos;
    glm::vec2 Scale;
    glm::vec4 Color;
    float Angle;
    //This could possibly be a pointer to Texture class instead or a index to GUID into a dict that references unique ptrs to it
    //GLuint TextureID;

    //Can be replaced with a weakptr instead and monitored if the texture was lost in case the user was dumb and deleted the texture before the quad
    Texture* Tex;
    //If quads were variable and we wanted reference to the Textures to be lost once quads are gone, we would use regular shared_ptrs 
    //but this class is cycled around a lot in stack and using shared_ptrs here would lead to MASSIVE performance loss because of all the reference counting
    
    //This const means that we are expecting this function to not mutate the member variables (Dont know why the const doesnt go to the front)
    std::array<Vertex, 4> ToVerts(uint32_t textureSlot) const;
    glm::mat4 ToMat();
};

class Rect
{
public:
    glm::vec2 Pos;
    /// <summary>
    /// Width/2 & Height/2 (Goes from -1 to 1)
    /// </summary>
    glm::vec2 Bounds;
    //std array here is very useful because we can pass the whole array by value (on stack)
    //In C it is also possible https://stackoverflow.com/questions/5157439/why-doesnt-c-support-functions-returning-arrays
    std::array<float, 4> GetBounds() const;

};
