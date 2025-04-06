#pragma once
#include "Encin/Tools/MathYaman.h"

namespace GamEncin
{
    class Object;
    class Renderer;
    class Camera;

    class Scene
    {
    public:
        Scene();

        Renderer* renderer = nullptr;
        vector<Object*> objects;

        template<typename T >
        Object* CreateObject()
        {
            Object* object = new Object(this);
            object->AddComponent<T>();
            AddObject(object);
            return object;
        }

        Object* CreateObject();
        Object* CreateObject(string name, string tag);
        Object* CreateAndUseCameraObject(Vector2Int size);
        void SetMainCamera(Camera* camera);
        void AddObject(Object* object);
        void RemoveObject(Object* object);
        void Clear();

        void Awake();
        void Start();
        void Update();
        void LateUpdate();
        void FixUpdate();
        void StartOfSecond();
    };
}
