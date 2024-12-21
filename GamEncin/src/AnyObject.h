#include "GamEncin.h"
#include <iostream>

class AnyObject : public Object
{
public:
    void Start() override
    {
        name = "myObject";
        std::cout << "Engine is started: " << name << "\n";
    }

    void Update() override
    {
        std::cout << "Engine is updated: " << name << "\n";
    }

    void FixUpdate() override
    {
        std::cout << "Engine is fix updated: " << name << "\n";
    }
};