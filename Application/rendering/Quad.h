#pragma once
//This can be a single header file (needs to be used in multiple locations so cant be in single translation unit)
#include <glm/vec2.hpp>
#include <glm/mat4x4.hpp>

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
};
