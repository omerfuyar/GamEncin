#pragma once
#include "Encin/Items/Components/Component.h"
#include "Encin/Tools/OpenGLObjects.h"
#include "Encin/Tools/Toolkit.h"

namespace GamEncin
{
    class Mesh : public Component
    {
    public:
        Mesh(Object* obj);
        ~Mesh();

        void SetChanged(bool value);
        void SetMeshData(MeshData* data);

        bool HasChanged();
        MeshData* const GetMeshData();

    private:
        bool hasChanged = true;

        MeshData* meshData = nullptr;
    };
}
