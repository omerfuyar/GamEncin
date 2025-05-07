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

    void TextMesh::SetFont(Font* font)
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
        string currentText = this->text;

        if(meshData)
        {
            // Renderer::RemoveMesh(this); // Assuming AddMesh/RemoveMesh is handled elsewhere or if meshData is rebuilt
            delete meshData;
            meshData = nullptr;
        }

        if(currentText.empty())
        {
            meshData = MeshBuilder::CreateMeshData({}, {});
            // Renderer::AddMesh(this); // Assuming AddMesh/RemoveMesh is handled elsewhere
            return;
        }

        unsigned int totalLines = 1;
        for(char c : currentText)
        {
            if(c == '\n')
            {
                totalLines++;
            }
        }

        unsigned int lineCount = 0;
        unsigned int quadCount = 0;
        unsigned int charIndexOnLine = 0;

        for(int i = 0; i < currentText.length(); i++)
        {
            if(currentText[i] == '\n')
            {
                lineCount++;
                charIndexOnLine = 0;
                continue;
            }

            Character character = font->chars[currentText[i]];

            Vector2 charUVTopLeftOrigin = character.uv;
            Vector2 charAtlasSize = character.size;
            Vector2 charAtlasOffset = character.offset;

            Vector2 displayCharSize = charAtlasSize * textSize;

            Vector2 charCenterPosition =
                Vector2(
                (((float) currentText.length() / -2.0f) + charIndexOnLine) * (charDistance / 50.0f),
                (((float) totalLines / -2.0f) - lineCount) * (lineDistance / 15.0f)) *
                textSize + charAtlasOffset;

            Vector2 posBottomLeft = Vector2(charCenterPosition.x - displayCharSize.x / 2.0f, charCenterPosition.y - displayCharSize.y / 2.0f);
            Vector2 posBottomRight = Vector2(charCenterPosition.x + displayCharSize.x / 2.0f, charCenterPosition.y - displayCharSize.y / 2.0f);
            Vector2 posTopRight = Vector2(charCenterPosition.x + displayCharSize.x / 2.0f, charCenterPosition.y + displayCharSize.y / 2.0f);
            Vector2 posTopLeft = Vector2(charCenterPosition.x - displayCharSize.x / 2.0f, charCenterPosition.y + displayCharSize.y / 2.0f);

            Vector2 uvTopLeft = charUVTopLeftOrigin;
            Vector2 uvTopRight = Vector2(charUVTopLeftOrigin.x + charAtlasSize.x, charUVTopLeftOrigin.y);
            Vector2 uvBottomLeft = Vector2(charUVTopLeftOrigin.x, charUVTopLeftOrigin.y + charAtlasSize.y);
            Vector2 uvBottomRight = Vector2(charUVTopLeftOrigin.x + charAtlasSize.x, charUVTopLeftOrigin.y + charAtlasSize.y);

            vertices.push_back(RawVertex(posBottomLeft, uvBottomLeft));
            vertices.push_back(RawVertex(posBottomRight, uvBottomRight));
            vertices.push_back(RawVertex(posTopRight, uvTopRight));
            vertices.push_back(RawVertex(posTopLeft, uvTopLeft));

            charIndexOnLine++;
            quadCount++;
        }

        vector<unsigned int> indices;

        for(int i = 0; i < quadCount; i++)
        {
            indices.push_back(i * 4);
            indices.push_back(i * 4 + 1);
            indices.push_back(i * 4 + 2);
            indices.push_back(i * 4 + 2);
            indices.push_back(i * 4 + 3);
            indices.push_back(i * 4);
        }

        meshData = MeshBuilder::CreateMeshData(vertices, indices);
        // Renderer::AddMesh(this);
    }

    void TextMesh::Update()
    {
        // UpdateTextMeshData();
    }
}
