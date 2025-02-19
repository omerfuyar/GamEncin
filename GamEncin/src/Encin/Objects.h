#pragma once
#include "Tools.h"

namespace GamEncin
{
    class Object
    {
    public:
        Object() {}
        virtual ~Object() {}

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
        virtual void Awake() {};
        virtual void Start() {};
        virtual void Update() {};
        virtual void FixUpdate() {};
    };

    class Scene
    {
    public:
        Scene();

        vector<Object*> objects;

        template <typename T>
        T& CreateObject()
        {
            T* object = new T();
            objects.push_back(dynamic_cast<Object*>(object));
            return *object;
        }

        void AddObject(Object& object);
        void RemoveObject(Object& object);
        void Clear();

        void Awake();
        void Start();
        void Update();
        void FixUpdate();
    };
}
