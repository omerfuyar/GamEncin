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
        void SetMeshTexture(Texture* texture);

        bool HasChanged();
        MeshData* const GetMeshData();
        Texture* const GetMeshTexture();

    private:
        bool hasChanged = true;

        MeshData* meshData = nullptr;
        Texture* meshTexture = nullptr;
    };
}
