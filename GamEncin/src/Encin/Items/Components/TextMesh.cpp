#include "Encin/Encin.h"

namespace GamEncin
{
    TextMesh::TextMesh(Object *obj) : Mesh(obj)
    {
    }

    void TextMesh::SetCharDistance(float distance)
    {
        this->charDistance = distance;
        UpdateTextMeshData();
    }

    void TextMesh::SetLineDistance(float distance)
    {
        this->lineDistance = distance;
        UpdateTextMeshData();
    }

    void TextMesh::SetText(string text)
    {
        this->text = text;
        UpdateTextMeshData();
    }

    void TextMesh::SetTextSize(Vector2 textSize)
    {
        this->textSize = textSize;
        UpdateTextMeshData();
    }

    void TextMesh::SetFont(Font *font)
    {
        this->font = font;
        texture = font->texture;
        UpdateTextMeshData();
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

    Font *const TextMesh::GetFont()
    {
        return font;
    }

    void TextMesh::UpdateTextMeshData()
    {
        if (!font)
        {
            Application::PrintLog(NullPointerErr, "Font is null");
            return;
        }

        vector<RawVertex> vertices;
        string currentText = this->text;

        if (meshData)
        {
            Renderer::RemoveMesh(this);
            delete meshData;
            meshData = nullptr;
        }

        if (currentText.empty())
        {
            meshData = MeshBuilder::CreateMeshData({}, {});
            if (Application::IsRunning())
            {
                Renderer::AddMesh(this);
            }
            return;
        }

        vector<string> lines = SplitString(currentText, "\n");
        unsigned int totalLines = lines.size();
        unsigned int quadCount = 0;

        unsigned int lineCountInText = lines.size();
        for (int lineIndex = 0; lineIndex < lines.size(); lineIndex++)
        {
            unsigned int charCountInLine = lines[lineIndex].length();

            for (int charIndex = 0; charIndex < lines[lineIndex].length(); charIndex++)
            {
                Character character = font->chars[lines[lineIndex][charIndex]];

                Vector2 charUVTopLeftOrigin_Original = character.uv;
                Vector2 charAtlasSize = character.size;
                Vector2 charAtlasOffset = character.offset;

                Vector2 displayCharSize = charAtlasSize * textSize;

                Vector2 charCenterPosition =
                    Vector2(
                        (((float)charCountInLine / -2.0f) + charIndex + 0.5f) * (charDistance / 25.0f),
                        (((float)totalLines / -2.0f) + (totalLines - 1 - lineIndex) + 0.5f) * (lineDistance / 15.0f)) *
                    textSize;

                Vector2 posBottomLeft = Vector2(charCenterPosition.x - displayCharSize.x / 2.0f, charCenterPosition.y - displayCharSize.y / 2.0f);
                Vector2 posBottomRight = Vector2(charCenterPosition.x + displayCharSize.x / 2.0f, charCenterPosition.y - displayCharSize.y / 2.0f);
                Vector2 posTopRight = Vector2(charCenterPosition.x + displayCharSize.x / 2.0f, charCenterPosition.y + displayCharSize.y / 2.0f);
                Vector2 posTopLeft = Vector2(charCenterPosition.x - displayCharSize.x / 2.0f, charCenterPosition.y + displayCharSize.y / 2.0f);

                Vector2 uvBottomLeft = Vector2(charUVTopLeftOrigin_Original.x, 1.0f - (charUVTopLeftOrigin_Original.y + charAtlasSize.y));
                Vector2 uvBottomRight = Vector2(charUVTopLeftOrigin_Original.x + charAtlasSize.x, 1.0f - (charUVTopLeftOrigin_Original.y + charAtlasSize.y));
                Vector2 uvTopRight = Vector2(charUVTopLeftOrigin_Original.x + charAtlasSize.x, 1.0f - charUVTopLeftOrigin_Original.y);
                Vector2 uvTopLeft = Vector2(charUVTopLeftOrigin_Original.x, 1.0f - charUVTopLeftOrigin_Original.y);

                vertices.push_back(RawVertex(posBottomLeft, uvBottomLeft));
                vertices.push_back(RawVertex(posBottomRight, uvBottomRight));
                vertices.push_back(RawVertex(posTopRight, uvTopRight));
                vertices.push_back(RawVertex(posTopLeft, uvTopLeft));

                quadCount++;
            }
        }

        vector<unsigned int> indices;

        for (int i = 0; i < quadCount; i++)
        {
            indices.push_back(i * 4);
            indices.push_back(i * 4 + 1);
            indices.push_back(i * 4 + 2);
            indices.push_back(i * 4 + 2);
            indices.push_back(i * 4 + 3);
            indices.push_back(i * 4);
        }

        meshData = MeshBuilder::CreateMeshData(vertices, indices);
        Renderer::AddMesh(this);
    }

    void TextMesh::Update()
    {
        // UpdateTextMeshData();
    }
}
