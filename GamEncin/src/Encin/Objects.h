#pragma once
#include "Tools.h"
#include <GLAD/glad.h>
#include <GLFW/glfw3.h>

namespace GamEncin
{
    class Object
    {
    public:
        string name = "Object",
            tag = "Default Tag";

        Layer layer = Default;

        float mass = 1,
            gravityScale = 1,
            drag = 0,
            angularDrag = 0;

        Vector3 position,
            rotation,
            scale = Vector3::One();

        vector<Vector3> vertices;

        void SendVerticesDataToBuffer(vector<Vector3> vertices);
        void Awake() {};
        void Start() {};
        void Update() {};
        void FixUpdate() {};
    };

    class Scene
    {
    public:
        Scene();

        vector<Object*> objects;

        Object& CreateObject();
        void AddObject(Object& object);
        void RemoveObject(Object& object);
        void Clear();

        void Awake();
        void Start();
        void Update();
        void FixUpdate();
    };
}
