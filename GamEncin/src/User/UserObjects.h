#pragma once 
#include "GamEncin.h"
#include <stdio.h>

class TestObject : public Object
{
public:
    TestObject() {}

    void Start() override
    {
        printf("TestObject Start\n");
    }

    void FixUpdate() override
    {
        printf("TestObject Fixupdate\n");
    }
};
