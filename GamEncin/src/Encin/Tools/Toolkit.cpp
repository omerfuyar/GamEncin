#include "Encin/Encin.h"
#define STB_IMAGE_IMPLEMENTATION
#include <STB/stb_image.h>
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <STB/stb_image_write.h>

namespace GamEncin
{
    namespace Toolkit
    {
#pragma region Texture Tools

#pragma region Texture

        Texture::Texture(unsigned int id, unsigned int bitsPerPixel, unsigned char *data, unsigned long long handle, Vector2Int size, string sourceFilePath)
        {
            this->id = id;
            this->bitsPerPixel = bitsPerPixel;
            this->data = data;
            this->size = size;
            this->handle = handle;
            this->sourceFilePath = sourceFilePath;
        }

        void Texture::SetWrapAndFilter(int wrapModeS, int wrapModeT, int minFilter, int magFilter)
        {
            this->wrapModeS = wrapModeS;
            this->wrapModeT = wrapModeT;
            this->minFilter = minFilter;
            this->magFilter = magFilter;

            if (!Application::IsRunning())
                return;

            glBindTexture(GL_TEXTURE_2D, id);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapModeS);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapModeT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilter);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magFilter);
        }

        void Texture::Initialize()
        {
            glGenTextures(1, &id);
            glBindTexture(GL_TEXTURE_2D, id);

            unsigned int imageFormat =
                (bitsPerPixel / 8 == 4)   ? GL_RGBA
                : (bitsPerPixel / 8 == 3) ? GL_RGB
                : (bitsPerPixel / 8 == 2) ? GL_RG
                                          : GL_RED;

            glTexImage2D(GL_TEXTURE_2D, 0, imageFormat, size.x, size.y, 0, imageFormat, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);

            SetWrapAndFilter(wrapModeS, wrapModeT, minFilter, magFilter);

            handle = glGetTextureHandleARB(id);
            glMakeTextureHandleResidentARB(handle);

            stbi_image_free(data);

            Application::PrintLog(LogType::Safe, "\nTexture successfully loaded: " + sourceFilePath);
        }

#pragma endregion

#pragma region TextureManager

        vector<Texture *> TextureManager::loadedTextures;

        Texture *TextureManager::GetTexture(string textureFilePath)
        {
            auto obj = std::find_if(loadedTextures.begin(), loadedTextures.end(), [&textureFilePath](Texture *texture)
                                    { return texture->sourceFilePath == textureFilePath; });

            if (obj != loadedTextures.end())
            {
                return *obj;
            }

            int imageWidth, imageHeight, imageChannels;
            stbi_set_flip_vertically_on_load(true);
            unsigned char *imageData = stbi_load(textureFilePath.c_str(), &imageWidth, &imageHeight, &imageChannels, 0);
            if (!imageData)
            {
                Application::PrintLog(IOErr, "Couldn't load texture file: " + textureFilePath);
                return nullptr;
            }

            Texture *texture = new Texture(0, imageChannels * 8, imageData, 0, Vector2Int(imageWidth, imageHeight), textureFilePath);

            if (Application::IsRunning())
            {
                texture->Initialize();
            }

            loadedTextures.push_back(texture);

            return texture;
        }

        void TextureManager::DeleteTexture(Texture *textureToDelete)
        {
            if (!textureToDelete)
            {
                Application::PrintLog(NullPointerErr, "Texture trying to delete is null");
                return;
            }

            auto obj = std::find(loadedTextures.begin(), loadedTextures.end(), textureToDelete);

            if (obj == loadedTextures.end())
            {
                Application::PrintLog(ElementCouldNotFindErr, "Texture to delete couldn't find");
                return;
            }

            glMakeTextureHandleNonResidentARB((*obj)->handle);
            glDeleteTextures(1, &(*obj)->id);

            loadedTextures.erase(obj);
        }

        void TextureManager::InitializeTextures()
        {
            for (Texture *texture : loadedTextures)
            {
                texture->Initialize();
            }
        }

#pragma endregion

#pragma endregion

#pragma region TextMesh Tools

#pragma region Character

        Character::Character(char character, Vector2 uv, Vector2 size, Vector2 offset)
        {
            this->character = character;
            this->uv = uv;
            this->size = size;
            this->offset = offset;
        }

#pragma endregion

#pragma region Font

        Font::Font(string name, Texture *sourceImage, unordered_map<char, Character> chars, string bdfFilePath)
        {
            this->name = name;
            this->texture = sourceImage;
            this->chars = chars;
            this->bdfFilePath = bdfFilePath;

            texture->SetWrapAndFilter(GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE, GL_NEAREST, GL_NEAREST);

            Application::PrintLog(LogType::Safe, "\nFont successfully loaded: " + bdfFilePath);
        }

#pragma endregion

#pragma region FontManager

        vector<Font *> FontManager::loadedFonts;
        Vector2Int FontManager::atlasSize = Vector2Int(256, 256);

        Font *FontManager::GetFont(string bdfFilePath)
        {
            auto obj = std::find_if(loadedFonts.begin(), loadedFonts.end(), [&bdfFilePath](Font *font)
                                    { return font->bdfFilePath == bdfFilePath; });

            if (obj != loadedFonts.end())
            {
                return *obj;
            }

            Font *createdFont = CreateFontFromBDF(bdfFilePath);

            if (!createdFont)
            {
                Application::PrintLog(IOErr, "Couldn't create font from file: " + bdfFilePath);
                return nullptr;
            }

            loadedFonts.push_back(createdFont);

            return createdFont;
        }

        void FontManager::DeleteFont(Font *fontToDelete)
        {
            if (!fontToDelete)
            {
                Application::PrintLog(NullPointerErr, "Font trying to delete is null");
                return;
            }

            auto obj = std::find(loadedFonts.begin(), loadedFonts.end(), fontToDelete);

            if (obj == loadedFonts.end())
            {
                Application::PrintLog(ElementCouldNotFindErr, "Font to delete couldn't find");
                return;
            }

            loadedFonts.erase(obj);
        }

        Font *const FontManager::CreateFontFromBDF(string bdfFilePath)
        {
            string fontName = "Default Font";
            unordered_map<char, Character> fontChars;

            vector<unsigned char> atlasData = vector<unsigned char>(atlasSize.x * atlasSize.y * 4, 0);
            Vector2Int cursor = Vector2Int(0, 0);
            int rowHeight = 0;

            string bdf = Input::GetFileContents(bdfFilePath.c_str());

            if (bdf.empty())
            {
                Application::PrintLog(IOErr, "Could not read BDF file or file is empty: " + bdfFilePath);
                return nullptr;
            }

            vector<string> lines = SplitString(bdf, "\n");

            // Find font name in the BDF file and set the name of the font
            for (const string &line : lines)
            {
                if (line.rfind("FONT ", 0) == 0)
                {
                    vector<string> parts = SplitString(line, " ");
                    string xlfdName = parts[1];
                    vector<string> tokens = SplitString(xlfdName, "-");
                    fontName = tokens[1] + "_" + tokens[2] + "_" + tokens[3]; // Family_Weight_Slant
                    break;
                }
            }

            int i = 0;
            while (i < lines.size()) // per line
            {
                if (lines[i].rfind("STARTCHAR", 0) == 0) // per char
                {
                    char currentChar = '?';
                    Vector2Int bbxSize;
                    Vector2Int bbxOffset;
                    vector<string> bitmapLines;
                    string charNameStr = SplitString(lines[i], " ")[1];

                    int startCharLine = i;

                    while (i < lines.size() && lines[i] != "ENDCHAR")
                    {
                        if (lines[i].rfind("ENCODING", 0) == 0)
                        {
                            currentChar = (char)StringToInt(SplitString(lines[i], " ")[1]);
                        }
                        else if (lines[i].rfind("BBX", 0) == 0)
                        {
                            vector<string> bbxWords = SplitString(lines[i], " ");

                            bbxSize.x = StringToInt(bbxWords[1]);
                            bbxSize.y = StringToInt(bbxWords[2]);
                            bbxOffset.x = StringToInt(bbxWords[3]);
                            bbxOffset.y = StringToInt(bbxWords[4]);
                        }
                        else if (lines[i] == "BITMAP")
                        {
                            i++; // Move to first line of bitmap data

                            while (i < lines.size() && lines[i] != "ENDCHAR")
                            {
                                bitmapLines.push_back(lines[i]);
                                i++;
                            }

                            // process bitmap data after all bitmap lines of the char collected
                            if (bbxSize.x > 0 && bbxSize.y > 0) // chars with bitmap data
                            {
                                // atlas packing
                                if (cursor.x + bbxSize.x > atlasSize.x)
                                {
                                    cursor.x = 0;
                                    cursor.y += rowHeight + 1; // Move to next row (1px padding)
                                    rowHeight = 0;
                                }

                                if (cursor.y + bbxSize.y > atlasSize.y) // no vertical space, todo this can be removed
                                {
                                    Application::Stop(IOErr, "Font atlas full. Character '" + charNameStr + "' (Encoding: " + currentChar + ") for font: " + fontName + ". Increase the atlas resolution to be able to output atlas texture.");
                                }

                                // Write bitmap data to atlas
                                for (int dataY = 0; dataY < bbxSize.y && dataY < bitmapLines.size(); dataY++)
                                {
                                    int bytesPerRow = (bbxSize.x + 7) / 8;
                                    string hexRow = bitmapLines[dataY];
                                    unsigned long long rowBits = 0;

                                    try
                                    {
                                        rowBits = std::stoull(hexRow, nullptr, 16);
                                    }
                                    catch (const std::invalid_argument &ia)
                                    {
                                        Application::PrintLog(TypeMismatchErr, "Invalid hex in BDF bitmap for char " + charNameStr + ": " + hexRow);
                                        continue; // Skip this row or char
                                    }

                                    for (int dataX = 0; dataX < bbxSize.x; dataX++)
                                    {
                                        int bitPosition = (bytesPerRow * 8) - 1 - dataX;
                                        if (bbxSize.x <= 64 && (rowBits >> bitPosition) & 1)
                                        {
                                            int pixelBaseIndex = ((cursor.y + dataY) * atlasSize.x + (cursor.x + dataX)) * 4;

                                            if (pixelBaseIndex + 3 < atlasData.size())
                                            {
                                                atlasData[pixelBaseIndex + 0] = 255; // R
                                                atlasData[pixelBaseIndex + 1] = 255; // G
                                                atlasData[pixelBaseIndex + 2] = 255; // B
                                                atlasData[pixelBaseIndex + 3] = 255; // A
                                            }
                                        }
                                    }
                                }

                                Vector2 uv = Vector2((float)cursor.x / atlasSize.x, (float)cursor.y / atlasSize.y);
                                Vector2 size = Vector2((float)bbxSize.x / atlasSize.x, (float)bbxSize.y / atlasSize.y);
                                Vector2 offset = Vector2((float)bbxOffset.x / atlasSize.x, (float)bbxOffset.y / atlasSize.y); // todo maybe dont make this normalized, but use the bbxSize instead
                                // Vector2 offset = Vector2((float) bbxOffset.x, (float) bbxOffset.y);

                                fontChars[currentChar] = Character(currentChar, uv, size, offset);

                                cursor.x += bbxSize.x + 1; // Advance cursor (1px padding)
                                rowHeight = Max(rowHeight, bbxSize.y);
                            }
                            else // chars with no bitmap data : ' ' etc.
                            {
                                Vector2 offset = Vector2((float)bbxOffset.x / atlasSize.x, (float)bbxOffset.y / atlasSize.y); // todo maybe dont make this normalized, but use the bbxSize instead
                                fontChars[currentChar] = Character(currentChar, Vector2(0, 0), Vector2(0, 0), offset);
                            }

                            break;
                        }

                        i++;
                    }
                }

                i++; // Next line in BDF
            }

            string outputDir = Input::GetExeFilePath() + "/Resources/Fonts/";
            string outputImagePath = outputDir + fontName + "_atlas.png";

            if (!stbi_write_png(outputImagePath.c_str(), atlasSize.x, atlasSize.y, 4, atlasData.data(), atlasSize.x * 4))
            {
                Application::PrintLog(IOErr, "Failed to write font atlas image: " + outputImagePath);
                return nullptr;
            }

            Application::PrintLog(LogType::Safe, "\nFont atlas successfuly written to: " + outputImagePath);

            Texture *texture = TextureManager::GetTexture(outputImagePath);

            if (!texture)
            {
                Application::PrintLog(TypeMismatchErr, "Failed to load font atlas texture after writing: " + outputImagePath);
                // Optionally, attempt to delete the problematic PNG: std::remove(outputImagePath.c_str());
                return nullptr;
            }

            return new Font(fontName, texture, fontChars, bdfFilePath);
        }

        void FontManager::SetAtlasSize(Vector2Int atlasSize)
        {
            FontManager::atlasSize.x = Abs(atlasSize.x);
            FontManager::atlasSize.y = Abs(atlasSize.y);
        }

#pragma endregion

#pragma endregion

#pragma region Mesh Tools

#pragma region Primitives

#pragma region RawVertex

        RawVertex::RawVertex(Vector3 position, Vector2 texture)
        {
            this->position = position;
            this->uv = texture;
        }

        void RawVertex::SetObjectId(unsigned int objectId)
        {
            this->objectId = objectId;
        }

        void RawVertex::AddNormal(Vector3 normal)
        {
            this->normal += normal;
        }

        void RawVertex::NormalizeNormal()
        {
            this->normal.Normalize();
        }

#pragma endregion

#pragma region Vertex

        Vertex::Vertex(unsigned int id, const RawVertex &rawVertex) : RawVertex(rawVertex)
        {
            this->id = id;
        }

        void Vertex::AddFace(Face *face)
        {
            faces.push_back(face);
        }

        void Vertex::AddEdge(Edge *edge)
        {
            edges.push_back(edge);
        }

        Face *Vertex::TryGetFace(unsigned int faceId)
        {
            return 0;
        }

        Edge *Vertex::TryGetEdge(unsigned int edgeId)
        {
            return 0;
        }

#pragma endregion

#pragma region Edge

        Edge::Edge(Vertex *startVertex, Vertex *endVertex)
        {
            id = MeshBuilder::GenerateEdgeId(startVertex, endVertex);

            this->startVertex = startVertex;
            this->endVertex = endVertex;

            startVertex->AddEdge(this);
            endVertex->AddEdge(this);
        }

#pragma endregion

#pragma region Face

        Face::Face(Vertex *vertex1, Vertex *vertex2, Vertex *vertex3)
        {
            id = MeshBuilder::GenerateFaceId(vertex1, vertex2, vertex3);

            vertices[0] = vertex1;
            vertices[1] = vertex2;
            vertices[2] = vertex3;

            vertex1->AddFace(this);
            vertex2->AddFace(this);
            vertex3->AddFace(this);

            Vector3 &v0 = vertices[0]->position;
            Vector3 &v1 = vertices[1]->position;
            Vector3 &v2 = vertices[2]->position;

            Vector3 edge1 = v1 - v0;
            Vector3 edge2 = v2 - v0;

            Vector3 faceNormal = edge1.Cross(edge2).Normalized();

            vertices[0]->AddNormal(faceNormal);
            vertices[1]->AddNormal(faceNormal);
            vertices[2]->AddNormal(faceNormal);
        }

        void Face::SetEdges(Edge *edge1, Edge *edge2, Edge *edge3)
        {
            edges[0] = edge1;
            edges[1] = edge2;
            edges[2] = edge3;
        }

        bool Face::IsEdgeDirectionMatchingFace(Edge *edge)
        {
            for (int i = 0; i < 3; ++i)
            {
                Vertex *a = vertices[i];
                Vertex *b = vertices[(i + 1) % 3];
                if (a == edge->startVertex && b == edge->endVertex)
                    return true;
            }
            return false;
        }

#pragma endregion

#pragma region

        MeshData::MeshData(vector<Vertex *> vertices, unordered_map<unsigned int, Edge *> edges, vector<Face *> faces)
        {
            this->vertices = vertices;
            this->edges = edges;
            this->faces = faces;
        }

        vector<RawVertex> MeshData::GetRawVertexArray()
        {
            vector<RawVertex> rawVertices;

            for (RawVertex *rawVertex : vertices)
            {
                rawVertices.push_back(*rawVertex);
            }

            return rawVertices;
        }

        vector<unsigned int> MeshData::GetIndexArray()
        {
            vector<unsigned int> indices;

            for (Face *face : faces)
            {
                indices.push_back(batchVertexOffset + face->vertices[0]->id);
                indices.push_back(batchVertexOffset + face->vertices[1]->id);
                indices.push_back(batchVertexOffset + face->vertices[2]->id);
            }

            return indices;
        }

        void MeshData::SetForBatch(unsigned int id, unsigned int batchVertexOffset, unsigned int batchIndexOffset)
        {
            this->id = id;
            this->batchVertexOffset = batchVertexOffset;
            this->batchIndexOffset = batchIndexOffset;

            for (Vertex *vertex : vertices)
            {
                vertex->SetObjectId(id);
            }
        }

        void MeshData::DeleteData()
        {
            for (Vertex *vert : vertices)
            {
                delete vert;
            }
            vertices.clear();

            for (auto &edge : edges)
            {
                delete edge.second;
            }
            edges.clear();

            for (Face *face : faces)
            {
                delete face;
            }
            faces.clear();
        }

#pragma endregion

#pragma endregion

#pragma region MeshBuilder

        MeshData *MeshBuilder::CreateMeshData(const vector<RawVertex> vertices, const vector<unsigned int> indices)
        {
            if (vertices.empty() || indices.empty())
            {
                return new MeshData();
            }

            vector<Vertex *> tempVertices;
            unordered_map<unsigned int, Edge *> tempEdges;
            vector<Face *> tempFaces;

            tempVertices.resize(vertices.size());

            int vertexCount = vertices.size();
            for (int i = 0; i < vertexCount; i++)
            {
                tempVertices[i] = new Vertex(i, vertices[i]);
            }

            int triangleCount = indices.size() / 3;
            for (int i = 0; i < triangleCount; i++) // for triangles
            {
                Vertex *faceVertices[3] = {tempVertices[indices[i * 3]],
                                           tempVertices[indices[i * 3 + 1]],
                                           tempVertices[indices[i * 3 + 2]]};

                Face *face = new Face(faceVertices[0], faceVertices[1], faceVertices[2]);
                // edges of face is empty

                for (int i = 0; i < 3; i++) // for face edges
                {
                    Vertex *startVert = faceVertices[i];
                    Vertex *endVert = faceVertices[(i + 1) % 3];

                    unsigned int edgeId = MeshBuilder::GenerateEdgeId(startVert, endVert);
                    auto it = tempEdges.find(edgeId);
                    Edge *edge = it == tempEdges.end() ? nullptr : it->second;

                    if (edge)
                    {
                        edge->rightFace = face;
                    }
                    else // if null
                    {
                        edge = new Edge(startVert, endVert);
                        edge->leftFace = face;
                        tempEdges[edgeId] = edge;
                    }

                    face->edges[i] = edge;
                }
                // edges of face and meshData is set

                tempFaces.push_back(face);
                // face of meshData is set
            }

            for (Vertex *vertex : tempVertices)
            {
                vertex->NormalizeNormal();
            }

            return new MeshData(tempVertices, tempEdges, tempFaces);
        }

        unsigned int MeshBuilder::GenerateFaceId(Vertex *vertex1, Vertex *vertex2, Vertex *vertex3)
        {
            array<unsigned int, 3> ids = {vertex1->id, vertex2->id, vertex3->id};
            sort(ids.begin(), ids.end());
            return ((ids[0] << 22) | (ids[1] << 11) | ids[2]);
        }

        unsigned int MeshBuilder::GenerateEdgeId(Vertex *vertex1, Vertex *vertex2)
        {
            array<unsigned int, 2> ids = {vertex1->id, vertex2->id};
            sort(ids.begin(), ids.end());
            return ((ids[0] << 16) | ids[1]);
        }

#pragma region Primitive Creators

        MeshData *MeshBuilder::CreatePlane(Vector2 size)
        {
            size /= 2.0f;

            vector<RawVertex> vertices =
                {
                    RawVertex(Vector3(-1, -1, 0) * size, Vector2(0, 0)),
                    RawVertex(Vector3(1, -1, 0) * size, Vector2(1, 0)),
                    RawVertex(Vector3(1, 1, 0) * size, Vector2(1, 1)),
                    RawVertex(Vector3(-1, 1, 0) * size, Vector2(0, 1)),
                };

            vector<unsigned int> indices = {0, 1, 2, 2, 3, 0};

            return CreateMeshData(vertices, indices);
        }

        MeshData *MeshBuilder::CreateCircle(float radius, int resolution)
        {
            if (resolution < 3)
            {
                Application::PrintLog(TypeMismatchErr, "Resolution can not be smaller then 3, value set to 3.");
                resolution = 3;
            }

            vector<RawVertex> vertices;
            vector<unsigned int> indices;

            float x, z;
            float sectorStep = 2 * PI / resolution; // radian
            float currSectorAngle;

            vertices.push_back(RawVertex(Vector3(0, 0, 0), Vector3(0, 0, 0) * 255));

            for (int i = 0; i < resolution; ++i)
            {
                currSectorAngle = i * sectorStep;

                x = radius * CosRad(currSectorAngle);
                z = radius * SinRad(currSectorAngle);

                Vector3 vec = Vector3(x, 0, z);
                vertices.push_back(RawVertex(vec, vec * 255));
            }

            for (int i = 0; i < resolution; i++)
            {
                indices.push_back(0); // center
                indices.push_back(i + 1);
                indices.push_back((i + 1) % resolution + 1);
            }

            return CreateMeshData(vertices, indices);
        }

        MeshData *MeshBuilder::CreateCuboid(Vector3 size)
        {
            vector<RawVertex> vertices =
                {
                    RawVertex(Vector3(-1, -1, -1) * size, Vector2(0, 0)),
                    RawVertex(Vector3(1, -1, -1) * size, Vector2(1, 0)),
                    RawVertex(Vector3(1, 1, -1) * size, Vector2(1, 1)),
                    RawVertex(Vector3(-1, 1, -1) * size, Vector2(0, 1)),
                    RawVertex(Vector3(-1, -1, 1) * size, Vector2(0, 0)),
                    RawVertex(Vector3(1, -1, 1) * size, Vector2(1, 0)),
                    RawVertex(Vector3(1, 1, 1) * size, Vector2(1, 1)),
                    RawVertex(Vector3(-1, 1, 1) * size, Vector2(0, 1))};

            vector<unsigned int> indices = {
                0, 1, 2, 2, 3, 0,
                4, 5, 6, 6, 7, 4,
                0, 1, 5, 5, 4, 0,
                1, 2, 6, 6, 5, 1,
                2, 3, 7, 7, 6, 2,
                3, 0, 4, 4, 7, 3};

            return CreateMeshData(vertices, indices);
        }

        MeshData *MeshBuilder::CreatePyramid(float height, float baseLength)
        {
            float xzCoord = baseLength / 2;
            float yQuarter = height / 4;

            vector<RawVertex> vertices =
                {
                    RawVertex(Vector3(0.0, yQuarter * 3, 0.0), Vector3(255, 0, 0)),
                    RawVertex(Vector3(-xzCoord, -yQuarter, xzCoord), Vector3(0, 255, 0)),
                    RawVertex(Vector3(xzCoord, -yQuarter, xzCoord), Vector3(0, 0, 255)),
                    RawVertex(Vector3(xzCoord, -yQuarter, -xzCoord), Vector3(255, 255, 0)),
                    RawVertex(Vector3(-xzCoord, -yQuarter, -xzCoord), Vector3(0, 255, 255))};

            vector<unsigned int> indices = {
                0, 1, 2, 0, 2, 3,
                0, 3, 4, 0, 4, 1,
                1, 2, 3, 3, 4, 1};

            return CreateMeshData(vertices, indices);
        }

        MeshData *MeshBuilder::CreateCylinder(float height, float radius, int resolution)
        {
            if (resolution < 3)
            {
                Application::PrintLog(TypeMismatchErr, "Resolution can not be smaller then 3, value set to 3.");
                resolution = 3;
            }

            vector<RawVertex> vertices;
            vector<unsigned int> indices;

            float x, y, z;
            float sectorStep = 2 * PI / resolution; // radian
            float currSectorAngle;

            for (int i = -1; i < 2; i += 2)
            {
                y = i * height / 2;
                vertices.push_back(RawVertex(Vector3(0, y, 0), Vector3(0, y, 0) * 255));

                for (int j = 0; j < resolution; ++j)
                {
                    currSectorAngle = j * sectorStep;

                    x = radius * CosRad(currSectorAngle);
                    z = radius * SinRad(currSectorAngle);

                    Vector3 vec = Vector3(x, y, z);

                    vertices.push_back(RawVertex(vec, vec * 255));
                }
            }

            int center2 = resolution + 1;
            for (int i = 0; i < resolution; ++i)
            {
                // circles
                indices.push_back(0); // center1
                indices.push_back(i + 1);
                indices.push_back((i + 1) % resolution + 1);
                indices.push_back(center2); // center2
                indices.push_back(center2 + i + 1);
                indices.push_back(center2 + (i + 1) % resolution + 1);

                // sides
                indices.push_back(i + 1);
                indices.push_back((i + 1) % resolution + 1);
                indices.push_back(i + resolution + 2);
                indices.push_back((i + 1) % resolution + 1);
                indices.push_back(i + resolution + 2);
                indices.push_back((i + 1) % resolution + resolution + 2);
            }

            return CreateMeshData(vertices, indices);
        }

        MeshData *MeshBuilder::CreateSphere(float radius, int resolution)
        {
            if (resolution < 3)
            {
                Application::PrintLog(TypeMismatchErr, "Resolution can not be smaller then 3, value set to 3.");
                resolution = 3;
            }

            vector<RawVertex> vertices;
            vector<unsigned int> indices;

            // TODO this is not compatible with shaders and normals
            // this sphere is rendering from top to bottom,

            float x, y, z, xz;
            float stackStep = PI / resolution;      // radian
            float sectorStep = 2 * PI / resolution; // radian
            float currStackAngle, currSectorAngle;

            for (int i = 0; i <= resolution; ++i)
            {
                currStackAngle = PI / 2 - i * stackStep; // from pi/2 to -pi/2 inclusive

                xz = radius * CosRad(currStackAngle);
                y = radius * SinRad(currStackAngle);

                for (int j = 0; j <= resolution; ++j)
                {
                    currSectorAngle = j * sectorStep; // from 0 to 2pi inclusive

                    x = xz * CosRad(currSectorAngle);
                    z = xz * SinRad(currSectorAngle);

                    Vector3 vec = Vector3(x, y, z);
                    vertices.push_back(RawVertex(vec, vec * 255));
                }
            }

            // generate CCW index list of sphere triangles
            int k1, k2;
            for (int i = 0; i < resolution; ++i)
            {
                k1 = i * (resolution + 1); // beginning of current stack
                k2 = k1 + resolution + 1;  // beginning of next stack

                for (int j = 0; j < resolution; ++j, ++k1, ++k2)
                {
                    // 2 triangles per sector excluding first and last stacks
                    // k1 => k2 => k1+1
                    if (i != 0)
                    {
                        indices.push_back(k1);
                        indices.push_back(k2);
                        indices.push_back(k1 + 1);
                    }

                    // k1+1 => k2 => k2+1
                    if (i != (resolution - 1))
                    {
                        indices.push_back(k1 + 1);
                        indices.push_back(k2);
                        indices.push_back(k2 + 1);
                    }
                }
            }

            return CreateMeshData(vertices, indices);
        }

        MeshData *MeshBuilder::CreateCone(float height, float radius, int resolution)
        {
            if (resolution < 3)
            {
                Application::PrintLog(TypeMismatchErr, "Resolution can not be smaller then 3, value set to 3.");
                resolution = 3;
            }

            vector<RawVertex> vertices;
            vector<unsigned int> indices;

            float x, z;
            float yQuarter = (float)height / 4.0f;
            float sectorStep = 2.0f * PI / (float)resolution; // radian
            float currSectorAngle;

            Vector3 vec = Vector3(0, -yQuarter, 0);
            vertices.push_back(RawVertex(vec, vec * 255)); // bottom center

            vec = Vector3(0, yQuarter * 3, 0);
            vertices.push_back(RawVertex(vec, vec * 255)); // top center

            for (int i = 0; i < resolution; ++i)
            {
                currSectorAngle = i * sectorStep;

                x = radius * CosRad(currSectorAngle);
                z = radius * SinRad(currSectorAngle);

                vec = Vector3(x, -yQuarter, z);
                vertices.push_back(RawVertex(vec, vec * 255));
            }

            unsigned int topCenter = 1;
            unsigned int bottomCenter = 0;
            for (unsigned int i = 2; i < resolution + 2; i++)
            {
                unsigned int nextIndex = (i + 1 > resolution + 1) ? 2 : i + 1;

                // Circle
                indices.push_back(i);
                indices.push_back(bottomCenter); // Bottom
                indices.push_back(nextIndex);

                // Sides
                indices.push_back(i);
                indices.push_back(topCenter); // Top
                indices.push_back(nextIndex);
            }

            return CreateMeshData(vertices, indices);
        }

        MeshData *MeshBuilder::CreateSimit(float radius, float halfThickness, int resolution)
        {
            if (resolution < 3)
            {
                Application::PrintLog(TypeMismatchErr, "Resolution can not be smaller then 3, value set to 3.");
                resolution = 3;
            }

            vector<RawVertex> vertices;
            vector<unsigned int> indices;

            float x, y, z;
            float sectorSteps = 2 * PI / (float)resolution; // radian
            float currMajorAngle, currMinorAngle;

            for (int i = 0; i < resolution; ++i)
            {
                currMajorAngle = i * sectorSteps;

                for (int j = 0; j < resolution; j++)
                {
                    currMinorAngle = j * sectorSteps;
                    x = (radius + halfThickness * CosRad(currMinorAngle)) * CosRad(currMajorAngle);
                    y = halfThickness * SinRad(currMinorAngle);
                    z = (radius + halfThickness * CosRad(currMinorAngle)) * SinRad(currMajorAngle);

                    Vector3 vec = Vector3(x, y, z);
                    vertices.push_back(RawVertex(vec, vec * 255));
                }
            }

            for (int i = 0; i < resolution; i++) // major
            {
                int currMajorIndex = (i % resolution) * resolution;
                int nextMajorIndex = ((i + 1) % resolution) * resolution;

                for (int j = 0; j < resolution; j++) // minor
                {
                    indices.push_back(currMajorIndex + j);
                    indices.push_back(nextMajorIndex + j);
                    indices.push_back(currMajorIndex + (j + 1) % resolution);
                    indices.push_back(nextMajorIndex + j);
                    indices.push_back(nextMajorIndex + (j + 1) % resolution);
                    indices.push_back(currMajorIndex + (j + 1) % resolution);
                }
            }

            return CreateMeshData(vertices, indices);
        }

#pragma endregion

#pragma endregion

#pragma endregion
    }
}
