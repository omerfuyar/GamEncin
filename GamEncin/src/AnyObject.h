#include "GamEncin.h"
#include <iostream>

class AnyObject : public Object
{
public:
    void Awake() override
    {

    }

    void Start() override
    {
        name = "myObject";
        position = Vector2::one;
        //position = Vector2::down;
        std::cout << "X: " << position.x << "Y: " << position.y << "\n";
    }

    void Update() override
    {
        std::cout << "X: " << position.x << "Y: " << position.y << "\n";
    }

    void FixUpdate() override
    {
        std::cout << "X: " << position.x << "Y: " << position.y << "\n";
    }
};