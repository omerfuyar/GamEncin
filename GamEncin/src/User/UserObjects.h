#pragma once 
#include "GamEncin.h"

class TestObject : public Object
{
public:
    TestObject() {}

    void Start() override
    {
        cout << "Derived Function";
    }

    void FixUpdate()override
    {
        cout << "Derived Function" << "\n";
    }
};
