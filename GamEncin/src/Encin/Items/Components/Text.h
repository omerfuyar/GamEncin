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
        void SetText(string text);
        void SetTextSize(Vector2 textSize);
        void SetFont(Font* font);

        bool HasChanged();
        string GetText();
        Vector2 GetTextSize();
        Font* const GetFont();
        vector<RawVertex> GetTextRawVertexArray();
        vector<unsigned int> GetTextIndexArray();

    private:
        bool hasChanged = true;
        string text = "";
        Vector2 textSize = Vector2(0, 0);

        Font* font = nullptr;
    };
}
