#pragma once 
#include "GamEncin.h"

class TestObject : Object
{
public:
    void Start()
    {
        cout << "Derived Function";
    }

    void Update()
    {
        cout << "Derived Function";
    }
};
