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
    Quad(glm::vec2 pos, glm::vec2 scale, float angle, Texture* tex, glm::vec4 color = { 1,1,1,1 });
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
    //Update this class is referenced around using refs not copys and therefore doesnt get destructed all that often. SharedPTrs are very reasonable to use

    //This const means that we are expecting this function to not mutate the member variables (Dont know why the const doesnt go to the front)
    std::array<Vertex, 4> ToVerts(uint32_t textureSlot) const;
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
    std::array<float, 4> GetExtents() const;

    //The reason why this can be constexpr because it has no conditions and is simply some set of CPU instructions that will not change based on the input
    constexpr bool Contains(const glm::vec2& p) const
    {
        return (p.x >= Pos.x && p.y >= Pos.y && p.x < Pos.x + Bounds.x && p.y < Pos.y + Bounds.y);
    }

    constexpr bool Contains(const Rect& o) const
    {
        //This is AABB

        //o will be a and this will be b
        /*
         *      +-----------+
         *      |b          |
         *      |           |
         * +----+---+       |
         * |a   |   |       |
         * |    |   |       |
         * |    |   |       |
         * |    |   |       |
         * |    |   |       |
         * +----+---+       |
         *      |           |
         *      +-----------+
         */

                //a.Left < b.Right        and a.Right > b.Left
        return (o.Pos.x < Pos.x + Bounds.x && o.Pos.x + o.Bounds.x > Pos.x &&
                o.Pos.y < Pos.y + Bounds.y && o.Pos.y + o.Bounds.y > Pos.y);

    }

};
