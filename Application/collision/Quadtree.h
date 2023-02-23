#pragma once
#include "../rendering/Quad.h"
class Quadtree
{
public:
    Quadtree(float worldSize = 1000.0f);

private:
    float _worldSize;
    //player.onDeath.subscribe(this);
    //callback(this)
};


class CollisionRect
{
public:
    //The ctor should ask for params that can be filled in with emplace back to solve nested object
    CollisionRect();
//private:
    Rect _rect;

};
