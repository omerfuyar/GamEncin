#include "Encin/Encin.h"

namespace GamEncin
{
    Text::Text(Object* obj) : Component(obj)
    {
    }

    Text::~Text()
    {
        Renderer::RemoveText(this);
    }

    void Text::SetChanged(bool value)
    {
        hasChanged = value;
    }

    void Text::SetText(string text)
    {
        this->text = text;
    }

    void Text::SetTextSize(Vector2 textSize)
    {
        this->textSize = textSize;
        SetChanged(true);
    }

    void Text::SetFont(Font* font)
    {
        this->font = font;
        SetChanged(true);
    }

    bool Text::HasChanged()
    {
        return hasChanged;
    }

    string Text::GetText()
    {
        return text;
    }

    Vector2 Text::GetTextSize()
    {
        return textSize;
    }

    Font* const Text::GetFont()
    {
        return font;
    }

    vector<RawVertex> Text::GetTextRawVertexArray()
    {
        vector<RawVertex> vertices;

        for(char c : text)
        {
            Vector2Int charUV = font->charUVs[c];
            Vector2Int charSize = font->sizePerChar;

            //todo pixel to texture ratio conversion

            vertices.push_back(RawVertex(Vector3(0, 0), charUV));
            vertices.push_back(RawVertex(Vector3(charSize.x, 0), charUV));
            vertices.push_back(RawVertex(Vector3(charSize.x, charSize.y), charUV));
            vertices.push_back(RawVertex(Vector3(0, charSize.y), charUV));
        }

        return vertices;
    }

    vector<unsigned int> Text::GetTextIndexArray()
    {
        vector<unsigned int> indices;

        for(int i = 0; i < text.length(); i++)
        {
            indices.push_back(i * 4);
            indices.push_back(i * 4 + 1);
            indices.push_back(i * 4 + 2);
            indices.push_back(i * 4 + 2);
            indices.push_back(i * 4 + 3);
            indices.push_back(i * 4);
        }

        return indices;
    }
}
