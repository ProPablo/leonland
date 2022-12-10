#pragma once
//This can be a single header file (needs to be used in multiple locations so cant be in single translation unit)
#include <glm/vec2.hpp>
#include <glm/mat4x4.hpp>
#include <array>
class Quad
{
public:
    glm::vec2 Pos;
    glm::vec2 Scale;
    float Angle;
    glm::mat4 ToMat();
};

class Rect
{
public:
    glm::vec2 Pos;
    glm::vec2 Bounds;
    //std array here is very useful because we can pass the whole array by value (on stack)
    //In C it is also possible https://stackoverflow.com/questions/5157439/why-doesnt-c-support-functions-returning-arrays
    std::array<float, 4> GetBounds();
    
};
