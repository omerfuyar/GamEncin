#pragma once
#include "Encin/Items/Components/Component.h"
#include "Encin/Tools/OpenGLObjects.h"
#include "Encin/Tools/Toolkit.h"

namespace GamEncin
{
    class Text : public Component
    {
    public:
        Text(Object* obj);
        ~Text();

        void SetChanged(bool value);
        void SetCharDistance(float distance);
        void SetLineDistance(float distance);
        void SetText(string text);
        void SetTextSize(Vector2 textSize);
        void SetFont(Font* font);

        bool HasChanged();
        float GetCharDistance();
        float GetLineDistance();
        string GetText();
        Vector2 GetTextSize();
        Font* const GetFont();
        MeshData* const GetTextMeshData();

        void UpdateTextMeshData();

    private:
        bool hasChanged = true;
        float charDistance = 1.0f;
        float lineDistance = 1.0f;

        string text = "";
        Vector2 textSize = Vector2::One();

        MeshData* textMeshData = nullptr;
        Font* font = nullptr;

        void Update() override;
    };
}
