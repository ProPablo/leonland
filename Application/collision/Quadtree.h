#pragma once
#include "../rendering/Quad.h"

enum class CollisionType
{
    DEFAULT = 0x00,
    PLAYER = 0x00,
};

//Implementation from 
// https://www.youtube.com/watch?v=ASAowY6yJII&t=1866s
// Does not need to be templated as it will yeild back to an entity anyway
class QuadTree
{
public:
    QuadTree(float worldSize = 1000.0f);

private:
    Rect _rect;
    std::array<Rect, 4> _childrenAreas{};
    std::array<std::shared_ptr<QuadTree>, 4> _children{};

    float _worldSize;
    // player.onDeath.subscribe(this);
    // callback(this)
};

//This may serve as a bounding box
class CollisionRect
{
public:
    // The ctor should ask for params that can be filled in with emplace back to solve nested object
    // CollisionRect();
    // private:
    Rect _rect;
    CollisionType type = CollisionType::DEFAULT;
    // Reference back to the entity for this collision rect
};
