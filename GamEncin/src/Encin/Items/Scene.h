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
        Object* const FindFirstObjectWithTag(string tag);
        vector<Object*> const FindObjectsWithTag(string tag);

        Object& CreateObject(string name = "Object", string tag = "Default", Layer layer = Layer::Default);
        Object& CreateAndUseCameraObject();
        void AddObject(Object* object);
        void RemoveObject(Object* object);
        void ClearScene();

        void Awake();
        void Start();
        void Update();
        void LateUpdate();
        void FixUpdate();
        void StartOfSecond();

    private:
        vector<Object*> objects;

        void UpdateTransforms();
    };
}
