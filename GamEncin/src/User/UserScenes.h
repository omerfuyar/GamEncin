#pragma once
#include "GamEncin.h"

class TestScene1 : public Scene
{
public:
    TestScene1(string name);

    void BuildScene() override;
};

class TestScene2 : public Scene
{
public:
    TestScene2(string name);

    void BuildScene() override;
};
