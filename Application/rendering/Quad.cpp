#include "Quad.h"

std::array<float, 4> Rect::GetBounds()
{
    std::array<float, 4> arr;
    arr[0] = Pos.x - Bounds.x;//left
    arr[1] = Pos.x + Bounds.x;//right
    arr[2] = Pos.y - Bounds.y;//up
    arr[3] = Pos.y + Bounds.y;//down
    return arr;
}