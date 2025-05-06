#pragma once
#include "Encin/Items/Components/Mesh.h"

namespace GamEncin
{
    class TextMesh : public Mesh
    {
    public:
        TextMesh(Object* obj);
        ~TextMesh();

        void SetCharDistance(float distance);
        void SetLineDistance(float distance);
        void SetText(string text);
        void SetTextSize(Vector2 textSize);
        void SetFont(Font* font);

        float GetCharDistance();
        float GetLineDistance();
        string GetText();
        Vector2 GetTextSize();
        Font* const GetFont();

        void UpdateTextMeshData();

    private:
        float charDistance = 1.0f;
        float lineDistance = 1.0f;

        string text = "";
        Vector2 textSize = Vector2::One();
        Font* font = nullptr;

        void Update() override;
    };
}
