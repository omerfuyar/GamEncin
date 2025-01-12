#pragma once
#include "Tools.h"
#include <GLAD/glad.h>
#include <GLFW/glfw3.h>

namespace GamEncin
{
    class Object
    {
        vector<Vector3> vertices;
        float mass;
        Vector3 position, rotation, scale;

        void SendVerticesDataToBuffer(vector<Vector3> vertices);
    };

    class Scene
    {
        vector<Object*> objects;
    };

    class SceneManager
    {
        Scene scene;
    };
}
