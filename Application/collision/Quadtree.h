#pragma once
#include "../rendering/Quad.h"
#include "../core/Log.h"

enum class CollisionType
{
    DEFAULT = 0x00,
    PLAYER = 0x00,
};

//enum class CollisionShape
//{
//    CIRCLE,
//    AABB,
//    POLYGON //Not supported lul
//};

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



//Can alert if it is within someoneElse as entity
// Uses event system to get subscribers and whatnot
//This may serve as a bounding box
class CollisionRect// : Subject
{
public:
    
    // The ctor should ask for params that can be filled in with emplace back to solve nested object
    // CollisionRect();
    // private:
    Rect _rect;
    CollisionType type = CollisionType::DEFAULT;
    CollisionShape* shape;

    //Refresh every frame
    bool isSolved = false;
    // Reference back to the entity for this collision rect
};


class CollisionShape
{
    //This is an abstract class or an interface
    virtual bool RunTest() = 0;
};

//Add these to the entity
class CollisionCircle: CollisionShape
{

};

class CollisionAABB: CollisionShape
{
    //If the Original Collision Rect actaully worked, means AABB succeeded as that uis just the bounding box
    bool RunTest()
    {
        log_dbg("Running Test in AABB class");
        return true;
    }

};
