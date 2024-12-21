#pragma once
#include <string>
using std::string;

namespace GamEncin
{
    class Object
    {
    protected:
        string name = "Default Object";

    public:
        Object() = default;
        virtual ~Object() = default;

        virtual void Start() {}
        virtual void Update() {}
        virtual void FixUpdate() {}
    };
}