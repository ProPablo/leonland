#include "Quad.h"
//Included to add glm::scale
#include <glm/gtc/type_ptr.hpp>

std::array<float, 4> Rect::GetBounds() const
{
    std::array<float, 4> arr;
    arr[0] = Pos.x - Bounds.x;//left
    arr[1] = Pos.x + Bounds.x;//right
    arr[2] = Pos.y - Bounds.y;//up
    arr[3] = Pos.y + Bounds.y;//down
    return arr;
}


Quad::Quad(glm::vec2 pos, glm::vec2 scale, float angle, Texture* tex, glm::vec4 color) : Pos(pos), Scale(scale), Angle(angle), Tex(tex), Color(color)
{
}

std::array<Vertex, 4> Quad::ToVerts(uint32_t textureSlot) const
{

    std::array<Vertex, 4> arr;
    const glm::vec3 corners[] = {
        { 0.5,  0.5, 0.0f,},
        {0.5, -0.5, 0.0f,},
        {-0.5, -0.5, 0.0f,},
        {-0.5, 0.5, 0.0f},
    };


    const float tex_coords[][2] = {
        {1.0f,1.0f},
        {1.0f,0.0f},
        {0.0f,0.0f},
        {0.0f,1.0f},
    };

    glm::mat4 transform(1.0f);
    transform = glm::translate(transform, glm::vec3(Pos, 0));
    transform = glm::rotate(transform, glm::radians(Angle), glm::vec3(0, 0, 1));


    auto spriteScale = Scale * ((glm::vec2)Tex->size / (float)Tex->pixelsPerUnit);
    
    transform = glm::scale(transform, glm::vec3(spriteScale, 1));

    for (int i = 0; i < 4; i++)
    {
        //The w value has to be 1
        auto pos = glm::value_ptr(transform * glm::vec4(corners[i], 1));
        std::copy(pos, pos + 3, arr[i].pos);
        //using copy is the same as memcpy just works better with iterators https://www.tutorialspoint.com/c_standard_library/c_function_memcpy.htm
        std::copy(std::begin(tex_coords[i]), std::end(tex_coords[i]), arr[i].texCoord);
        auto color = glm::value_ptr(Color);
        std::copy(color, color + 4, arr[i].color);
        arr[i].texIndex = float(textureSlot);
    }


    float vertices[] = {
        //positions		//color			 //text_coord	
          1.0f, 0.0f, 0.0f,     1.0f,1.0f,			//top right,
            0.0f, 1.0f, 0.0f,	1.0f,0.0f,	//bottom right
            0.0f, 0.0f, 1.0f,	0.0f,0.0f,	//bottom left
            0.0f, 1.0f, 1.0f,	0.0f,1.0f,	//top left
    };


    return arr;
}
