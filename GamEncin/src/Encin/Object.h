#pragma once
#include "Tools.h"
#include <string>
using std::string;

namespace GamEncin
{
    class Object
    {
    protected:
        string name = "Object";
        Vector2 position;
        
    public:
        Object() = default;
        virtual ~Object() = default;

        virtual void Start() {}
        virtual void Update() {}
        virtual void FixUpdate() {}
    };
}