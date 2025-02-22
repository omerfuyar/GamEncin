#pragma once 
#include "GamEncin.h"

class TestObject : public Object
{
public:
    TestObject() {}

    void Start() override
    {
        //printf("asd\n"); works fine
    }

    void FixUpdate() override
    {
        //printf("qwe\n"); works fine
    }
};
