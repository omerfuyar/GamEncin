#pragma once
#include "Tools.h"
#include <string>
using std::string;

namespace GamEncin
{
    class Object
    {
    public:
        Layer layer = Default;
        string name = "Object";
        Vector2 position;

        Object() = default;
        virtual ~Object() = default;

        virtual void Awake() {}
        virtual void Start() {}
        virtual void Update() {}
        virtual void FixUpdate() {}


    };
}