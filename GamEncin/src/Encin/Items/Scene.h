#pragma once
#include "Encin/Tools/MathYaman.h"

namespace GamEncin
{
    class Object;
    class Renderer;
    class Camera;

    class Scene
    {
    private:
        vector<Object*> objects;

    public:
        Object& CreateObject();
        Object& CreateObject(string name, string tag);
        Object& CreateAndUseCameraObject();
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
