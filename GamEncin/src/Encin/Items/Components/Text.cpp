#include "Encin/Encin.h"

namespace GamEncin
{
    Text::Text(Object* obj) : Component(obj)
    {
    }

    Text::~Text()
    {
        Renderer::RemoveMesh(textMeshData);
    }

    void Text::SetChanged(bool value)
    {
        hasChanged = value;
    }

    void Text::SetCharDistance(float distance)
    {
        this->charDistance = distance;
        SetChanged(true);
    }

    void Text::SetLineDistance(float distance)
    {
        this->lineDistance = distance;
        SetChanged(true);
    }

    void Text::SetText(string text)
    {
        this->text = text;
        SetChanged(true);
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

    float Text::GetCharDistance()
    {
        return charDistance;
    }

    float Text::GetLineDistance()
    {
        return lineDistance;
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

    MeshData* const Text::GetTextMeshData()
    {
        return textMeshData;
    }

    void Text::UpdateTextMeshData()
    {
        if(!font)
        {
            Application::PrintLog(NullPointerErr, "Font is null");
            return;
        }

        vector<RawVertex> vertices;

        int totalLines = SplitString(text, "\n").size();
        int lineCount = 0;
        for(int i = 0; i < text.length(); i++)
        {
            if(text[i] == '\n')
            {
                lineCount++;
                continue;
            }

            Character character = font->chars[text[i]];
            Vector2 charUV = character.uv;
            Vector2 charSize = character.size;
            Vector2 charOffset = character.offset;

            Vector2 charPosition = Vector2(((float) text.length() / -2 + i) * charDistance, ((float) totalLines / -2 + lineCount)) * textSize;

            vertices.push_back(RawVertex(charPosition - charSize / 2, charUV));
            vertices.push_back(RawVertex(Vector2(charPosition.x + charSize.x / 2, charPosition.y - charSize.y / 2), Vector2(charUV.x + charSize.x, charUV.y)));
            vertices.push_back(RawVertex(charPosition + charSize / 2, charUV + charSize));
            vertices.push_back(RawVertex(Vector2(charPosition.x - charSize.x / 2, charPosition.y + charSize.y / 2), Vector2(charUV.x, charUV.y + charSize.x)));
        }

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

        if(textMeshData)
        {
            Renderer::RemoveMesh(textMeshData);
            printf("Text GetTextMeshData remove: %p\n", textMeshData);
        }

        textMeshData = MeshBuilder::CreateMeshData(font->texture, object->GetTransform()->GetModelMatrix(), vertices, indices);
        printf("Text GetTextMeshData add: %p\n", textMeshData);
        Renderer::AddMesh(textMeshData);

        SetChanged(false);
    }

    void Text::Update()
    {
        if(HasChanged())
        {
            UpdateTextMeshData();
        }
    }
}
