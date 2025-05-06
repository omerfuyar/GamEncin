#include "Encin/Encin.h"

namespace GamEncin
{
    TextMesh::TextMesh(Object* obj) : Mesh(obj)
    {
    }

    TextMesh::~TextMesh()
    {
        Renderer::RemoveMesh(this);
    }

    void TextMesh::SetCharDistance(float distance)
    {
        this->charDistance = distance;
    }

    void TextMesh::SetLineDistance(float distance)
    {
        this->lineDistance = distance;
    }

    void TextMesh::SetText(string text)
    {
        this->text = text;
        UpdateTextMeshData();
    }

    void TextMesh::SetTextSize(Vector2 textSize)
    {
        this->textSize = textSize;
    }

    void TextMesh::SetFont(Font* font)
    {
        this->font = font;
    }

    float TextMesh::GetCharDistance()
    {
        return charDistance;
    }

    float TextMesh::GetLineDistance()
    {
        return lineDistance;
    }

    string TextMesh::GetText()
    {
        return text;
    }

    Vector2 TextMesh::GetTextSize()
    {
        return textSize;
    }

    Font* const TextMesh::GetFont()
    {
        return font;
    }

    void TextMesh::UpdateTextMeshData()
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

        if(meshData)
        {
            Renderer::RemoveMesh(this);
            printf("Text GetTextMeshData remove: %p\n", meshData);
        }

        meshData = MeshBuilder::CreateMeshData(vertices, indices);
        printf("Text GetTextMeshData add: %p\n", meshData);
        Renderer::AddMesh(this);
    }

    void TextMesh::Update()
    {
        UpdateTextMeshData();
    }
}
