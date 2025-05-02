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

        Texture::Texture(unsigned int id, unsigned int bitsPerPixel, unsigned char* data, unsigned long long handle, Vector2Int size, string filePath)
        {
            this->id = id;
            this->bitsPerPixel = bitsPerPixel;
            this->data = data;
            this->size = size;
            this->handle = handle;
            this->filePath = filePath;
        }

        void Texture::Initialize()
        {
            glGenTextures(1, &id);
            glBindTexture(GL_TEXTURE_2D, id);

            unsigned int imageFormat = (bitsPerPixel / 8 == 4) ? GL_RGBA : (bitsPerPixel / 8 == 3) ? GL_RGB
                : (bitsPerPixel / 8 == 2) ? GL_RG
                : GL_RED;

            glTexImage2D(GL_TEXTURE_2D, 0, imageFormat, size.x, size.y, 0, imageFormat, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

            handle = glGetTextureHandleARB(id);
            glMakeTextureHandleResidentARB(handle);

            stbi_image_free(data);

            // printf("Texture %s loaded with id: %u\n", filePath.c_str(), id);
            // printf("Texture %s handle: %llu\n", filePath.c_str(), handle);
            // printf("Texture %s size: %d x %d\n", filePath.c_str(), size.x, size.y);
            // printf("Texture %s bits per pixel: %u\n", filePath.c_str(), bitsPerPixel);
            // printf("Texture %s data size: %d\n", filePath.c_str(), size.x * size.y * (bitsPerPixel / 8));
        }

#pragma endregion

#pragma region TextureManager

        vector<Texture*> TextureManager::loadedTextures;

        Texture* TextureManager::GetTexture(string textureFilePath)
        {
            auto obj = std::find_if(loadedTextures.begin(), loadedTextures.end(), [&textureFilePath](Texture* texture)
                                    { return texture->filePath == textureFilePath; });

            if(obj != loadedTextures.end())
            {
                return *obj;
            }

            int imageWidth, imageHeight, imageChannels;
            unsigned char* imageData = stbi_load(textureFilePath.c_str(), &imageWidth, &imageHeight, &imageChannels, 0);
            if(!imageData)
            {
                Application::PrintLog(IOErr, "Couldn't load texture file: " + textureFilePath);
                return nullptr;
            }

            Texture* texture = new Texture(0, imageChannels * 8, imageData, 0, Vector2Int(imageWidth, imageHeight), textureFilePath);

            if(Application::IsRunning())
            {
                texture->Initialize();
            }

            loadedTextures.push_back(texture);

            return texture;
        }

        void TextureManager::DeleteTexture(Texture* textureToDelete)
        {
            if(!textureToDelete)
            {
                Application::PrintLog(NullPointerErr, "Texture trying to delete is null");
                return;
            }

            auto obj = std::find(loadedTextures.begin(), loadedTextures.end(), textureToDelete);

            if(obj == loadedTextures.end())
            {
                Application::PrintLog(ElementCouldNotFoundErr, "Texture to delete couldn't find");
                return;
            }

            glMakeTextureHandleNonResidentARB((*obj)->handle);
            glDeleteTextures(1, &(*obj)->id);

            loadedTextures.erase(obj);
        }

        void TextureManager::InitializeTextures()
        {
            for(Texture* texture : loadedTextures)
            {
                texture->Initialize();
            }
        }

#pragma endregion

#pragma endregion

#pragma region Text Tools

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

        Font::Font(string name, Texture* sourceImage, unordered_map<char, Character> chars, string bdfFilePath)
        {
            this->name = name;
            this->texture = sourceImage;
            this->chars = chars;
            this->bdfFilePath = bdfFilePath;
        }

#pragma endregion

#pragma region FontManager

        vector<Font*> FontManager::loadedFonts;

        Font* FontManager::GetFont(string bdfFilePath)
        {
            auto obj = std::find_if(loadedFonts.begin(), loadedFonts.end(), [&bdfFilePath](Font* font)
                                    { return font->bdfFilePath == bdfFilePath; });

            if(obj != loadedFonts.end())
            {
                return *obj;
            }

            Font* createdFont = CreateFontFromBDF(bdfFilePath);

            loadedFonts.push_back(createdFont);

            return createdFont;
        }

        void FontManager::DeleteFont(Font* fontToDelete)
        {
            if(!fontToDelete)
            {
                Application::PrintLog(NullPointerErr, "Font trying to delete is null");
                return;
            }

            auto obj = std::find(loadedFonts.begin(), loadedFonts.end(), fontToDelete);

            if(obj == loadedFonts.end())
            {
                Application::PrintLog(ElementCouldNotFoundErr, "Font to delete couldn't find");
                return;
            }

            loadedFonts.erase(obj);
        }

        Font* const FontManager::CreateFontFromBDF(string bdfFilePath)
        {
            string fontName = "";
            unordered_map<char, Character> fontChars;

            int atlasWidth = 255, atlasHeight = 255;
            vector<unsigned char> atlasData(atlasWidth * atlasHeight, 0); // 1-channel monochrome
            Vector2Int cursor;
            int rowHeight = 0;

            string bdf = Input::GetFileContents(bdfFilePath.c_str());
            vector<string> lines = SplitString(bdf, "\n");

            vector<string> words = SplitString(lines[1], " ");
            vector<string> tokens = SplitString(words[1], "-");
            fontName = tokens[1] + "_" + tokens[2] + "_" + tokens[3];

            int i = 0;
            while(i < lines.size()) // per line
            {
                if(lines[i].rfind("STARTCHAR", 0) == 0) // per char
                {
                    char character = '?';
                    int dwidth = 0;
                    Vector2Int bbxSize;
                    Vector2Int bbxOffset;
                    vector<string> bitmapLines;

                    while(lines[i] != "ENDCHAR")
                    {
                        if(lines[i].rfind("ENCODING", 0) == 0)
                        {
                            character = (char) StringToInt(SplitString(lines[i], " ")[1]);
                        }
                        else if(lines[i].rfind("DWIDTH", 0) == 0)
                        {
                            dwidth = StringToInt(SplitString(lines[i], " ")[1]);
                        }
                        else if(lines[i].rfind("BBX", 0) == 0)
                        {
                            vector<string> words = SplitString(lines[i], " ");
                            //bbxSize.x = dwidth;
                            bbxSize.x = StringToInt(words[1]);
                            bbxSize.y = StringToInt(words[2]);
                            bbxOffset.x = StringToInt(words[3]);
                            bbxOffset.y = StringToInt(words[4]);
                        }
                        else if(lines[i] == "BITMAP")
                        {
                            i++;

                            while(i < lines.size() && lines[i] != "ENDCHAR")
                            {
                                bitmapLines.push_back(lines[i]);
                                i++;
                            }

                            break;
                        }

                        i++;
                    }

                    for(int y = 0; y < bbxSize.y; y++)
                    {
                        if(y >= bitmapLines.size())
                        {
                            break;
                        }

                        unsigned int rowBits = std::stoul(bitmapLines[y], nullptr, 16);

                        for(int x = 0; x < bbxSize.x; x++)
                        {
                            if((rowBits >> (bbxSize.x - 1 - x)) & 1)
                            {
                                atlasData[(cursor.y + y) * atlasWidth + (cursor.x + x)] = 255;
                            }
                        }
                    }

                    Vector2 uv = Vector2((float) cursor.x / atlasWidth, (float) cursor.y / atlasHeight);
                    Vector2 size = Vector2((float) bbxSize.x / atlasWidth, (float) bbxSize.y / atlasHeight);
                    Vector2 offset = Vector2((float) bbxOffset.x / atlasWidth, (float) bbxOffset.y / atlasHeight);

                    fontChars[character] = Character(character, uv, size, offset);

                    rowHeight = Max(rowHeight, bbxSize.y);
                    cursor.x += bbxSize.x + 1;

                    if(cursor.x + bbxSize.x >= atlasWidth)
                    {
                        cursor.x = 0;
                        cursor.y += rowHeight + 1;
                        rowHeight = 0;
                    }
                }

                i++;
            }

            string outputImagePath = "GamEncin/Resources/Fonts/" + fontName + "_font_atlas.png";

            if(!stbi_write_png(outputImagePath.c_str(), atlasWidth, atlasHeight, 1, atlasData.data(), atlasWidth))
            {
                Application::PrintLog(IOErr, "Failed to write font atlas image.");
            }
            else
            {
                Application::PrintLog(LogType::Safe, "Font atlas written to: " + outputImagePath + "\n");
            }

            Texture* texture = TextureManager::GetTexture(outputImagePath);

            return new Font(fontName, texture, fontChars, bdfFilePath);
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

        Vertex::Vertex(unsigned int id, const RawVertex& rawVertex) : RawVertex(rawVertex)
        {
            this->id = id;
        }

        void Vertex::AddFace(Face* face)
        {
            faces.push_back(face);
        }

        void Vertex::AddEdge(Edge* edge)
        {
            edges.push_back(edge);
        }

        Face* Vertex::TryGetFace(unsigned int faceId)
        {
            return 0;
        }

        Edge* Vertex::TryGetEdge(unsigned int edgeId)
        {
            return 0;
        }

#pragma endregion

#pragma region Edge

        Edge::Edge(Vertex* startVertex, Vertex* endVertex)
        {
            id = MeshBuilder::GenerateEdgeId(startVertex, endVertex);

            this->startVertex = startVertex;
            this->endVertex = endVertex;

            startVertex->AddEdge(this);
            endVertex->AddEdge(this);
        }

#pragma endregion

#pragma region Face

        Face::Face(Vertex* vertex1, Vertex* vertex2, Vertex* vertex3)
        {
            id = MeshBuilder::GenerateFaceId(vertex1, vertex2, vertex3);

            vertices[0] = vertex1;
            vertices[1] = vertex2;
            vertices[2] = vertex3;

            vertex1->AddFace(this);
            vertex2->AddFace(this);
            vertex3->AddFace(this);

            Vector3& v0 = vertices[0]->position;
            Vector3& v1 = vertices[1]->position;
            Vector3& v2 = vertices[2]->position;

            Vector3 edge1 = v1 - v0;
            Vector3 edge2 = v2 - v0;

            Vector3 faceNormal = edge1.Cross(edge2).Normalized();

            vertices[0]->AddNormal(faceNormal);
            vertices[1]->AddNormal(faceNormal);
            vertices[2]->AddNormal(faceNormal);
        }

        void Face::SetEdges(Edge* edge1, Edge* edge2, Edge* edge3)
        {
            edges[0] = edge1;
            edges[1] = edge2;
            edges[2] = edge3;
        }

        bool Face::IsEdgeDirectionMatchingFace(Edge* edge)
        {
            for(int i = 0; i < 3; ++i)
            {
                Vertex* a = vertices[i];
                Vertex* b = vertices[(i + 1) % 3];
                if(a == edge->startVertex && b == edge->endVertex)
                    return true;
            }
            return false;
        }

#pragma endregion

#pragma region MeshData

        MeshData::MeshData(Texture* texture, Matrix4* modelMatrix, vector<Vertex*> vertices, unordered_map<unsigned int, Edge*> edges, vector<Face*> faces)
        {
            this->texture = texture;
            this->modelMatrix = modelMatrix;
            this->vertices = vertices;
            this->edges = edges;
            this->faces = faces;
        }

        void MeshData::SetForBatch(unsigned int id, unsigned int batchVertexOffset, unsigned int batchIndexOffset)
        {
            this->id = id;
            this->batchVertexOffset = batchVertexOffset;
            this->batchIndexOffset = batchIndexOffset;

            for(Vertex* vertex : vertices)
            {
                vertex->SetObjectId(id);
            }
        }

        vector<RawVertex> MeshData::GetRawVertexArray()
        {
            vector<RawVertex> rawVertices;

            for(RawVertex* rawVertex : vertices)
            {
                rawVertices.push_back(*rawVertex);
            }

            return rawVertices;
        }

        vector<unsigned int> MeshData::GetIndexArray()
        {
            vector<unsigned int> indices;

            for(Face* face : faces)
            {
                indices.push_back(batchVertexOffset + face->vertices[0]->id);
                indices.push_back(batchVertexOffset + face->vertices[1]->id);
                indices.push_back(batchVertexOffset + face->vertices[2]->id);
            }

            return indices;
        }

        Texture* const MeshData::GetTexture()
        {
            return texture;
        }

        Matrix4* const MeshData::GetModelMatrix()
        {
            return modelMatrix;
        }

        void MeshData::DeleteData()
        {
            for(Vertex* vert : vertices)
            {
                delete vert;
            }
            vertices.clear();

            for(auto& edge : edges)
            {
                delete edge.second;
            }
            edges.clear();

            for(Face* face : faces)
            {
                delete face;
            }
            faces.clear();
        }

#pragma endregion

#pragma endregion

#pragma region MeshBuilder

        MeshData* MeshBuilder::CreateMeshData(Texture* const texture, Matrix4* const modelMatrix, const vector<RawVertex> vertices, const vector<unsigned int> indices)
        {
            vector<Vertex*> tempVertices;
            unordered_map<unsigned int, Edge*> tempEdges;
            vector<Face*> tempFaces;

            tempVertices.resize(vertices.size());

            int vertexCount = vertices.size();
            for(int i = 0; i < vertexCount; i++)
            {
                tempVertices[i] = new Vertex(i, vertices[i]);
            }

            int triangleCount = indices.size() / 3;
            for(int i = 0; i < triangleCount; i++) // for triangles
            {
                Vertex* faceVertices[3] = {tempVertices[indices[i * 3]],
                                           tempVertices[indices[i * 3 + 1]],
                                           tempVertices[indices[i * 3 + 2]]};

                Face* face = new Face(faceVertices[0], faceVertices[1], faceVertices[2]);
                // edges of face is empty

                for(int i = 0; i < 3; i++) // for face edges
                {
                    Vertex* startVert = faceVertices[i];
                    Vertex* endVert = faceVertices[(i + 1) % 3];

                    unsigned int edgeId = MeshBuilder::GenerateEdgeId(startVert, endVert);
                    auto it = tempEdges.find(edgeId);
                    Edge* edge = it == tempEdges.end() ? nullptr : it->second;

                    if(edge)
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

            for(Vertex* vertex : tempVertices)
            {
                vertex->NormalizeNormal();
            }

            return new MeshData(texture, modelMatrix, tempVertices, tempEdges, tempFaces);
        }

        unsigned int MeshBuilder::GenerateFaceId(Vertex* vertex1, Vertex* vertex2, Vertex* vertex3)
        {
            array<unsigned int, 3> ids = {vertex1->id, vertex2->id, vertex3->id};
            sort(ids.begin(), ids.end());
            return ((ids[0] << 22) | (ids[1] << 11) | ids[2]);
        }

        unsigned int MeshBuilder::GenerateEdgeId(Vertex* vertex1, Vertex* vertex2)
        {
            array<unsigned int, 2> ids = {vertex1->id, vertex2->id};
            sort(ids.begin(), ids.end());
            return ((ids[0] << 16) | ids[1]);
        }

#pragma region Primitive Creators

        MeshData* MeshBuilder::CreatePlane(Texture* const texture, Matrix4* const modelMatrix, Vector2 size)
        {
            vector<RawVertex> vertices =
            {
                RawVertex(Vector3(-1, -1, 0) * size, Vector2(0, 0)),
                RawVertex(Vector3(1, -1, 0) * size, Vector2(1, 0)),
                RawVertex(Vector3(1, 1, 0) * size, Vector2(1, 1)),
                RawVertex(Vector3(-1, 1, 0) * size, Vector2(0, 1)),
            };

            vector<unsigned int> indices = {0, 1, 2, 2, 3, 0};

            return CreateMeshData(texture, modelMatrix, vertices, indices);
        }

        MeshData* MeshBuilder::CreateCircle(Texture* const texture, Matrix4* const modelMatrix, float radius, int resolution)
        {
            if(resolution < 3)
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

            for(int i = 0; i < resolution; ++i)
            {
                currSectorAngle = i * sectorStep;

                x = radius * CosRad(currSectorAngle);
                z = radius * SinRad(currSectorAngle);

                Vector3 vec = Vector3(x, 0, z);
                vertices.push_back(RawVertex(vec, vec * 255));
            }

            for(int i = 0; i < resolution; i++)
            {
                indices.push_back(0); // center
                indices.push_back(i + 1);
                indices.push_back((i + 1) % resolution + 1);
            }

            return CreateMeshData(texture, modelMatrix, vertices, indices);
        }

        MeshData* MeshBuilder::CreateCuboid(Texture* const texture, Matrix4* const modelMatrix, Vector3 size)
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

            return CreateMeshData(texture, modelMatrix, vertices, indices);
        }

        MeshData* MeshBuilder::CreatePyramid(Texture* const texture, Matrix4* const modelMatrix, float height, float baseLength)
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

            return CreateMeshData(texture, modelMatrix, vertices, indices);
        }

        MeshData* MeshBuilder::CreateCylinder(Texture* const texture, Matrix4* const modelMatrix, float height, float radius, int resolution)
        {
            if(resolution < 3)
            {
                Application::PrintLog(TypeMismatchErr, "Resolution can not be smaller then 3, value set to 3.");
                resolution = 3;
            }

            vector<RawVertex> vertices;
            vector<unsigned int> indices;

            float x, y, z;
            float sectorStep = 2 * PI / resolution; // radian
            float currSectorAngle;

            for(int i = -1; i < 2; i += 2)
            {
                y = i * height / 2;
                vertices.push_back(RawVertex(Vector3(0, y, 0), Vector3(0, y, 0) * 255));

                for(int j = 0; j < resolution; ++j)
                {
                    currSectorAngle = j * sectorStep;

                    x = radius * CosRad(currSectorAngle);
                    z = radius * SinRad(currSectorAngle);

                    Vector3 vec = Vector3(x, y, z);

                    vertices.push_back(RawVertex(vec, vec * 255));
                }
            }

            int center2 = resolution + 1;
            for(int i = 0; i < resolution; ++i)
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

            return CreateMeshData(texture, modelMatrix, vertices, indices);
        }

        MeshData* MeshBuilder::CreateSphere(Texture* const texture, Matrix4* const modelMatrix, float radius, int resolution)
        {
            if(resolution < 3)
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

            for(int i = 0; i <= resolution; ++i)
            {
                currStackAngle = PI / 2 - i * stackStep; // from pi/2 to -pi/2 inclusive

                xz = radius * CosRad(currStackAngle);
                y = radius * SinRad(currStackAngle);

                for(int j = 0; j <= resolution; ++j)
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
            for(int i = 0; i < resolution; ++i)
            {
                k1 = i * (resolution + 1); // beginning of current stack
                k2 = k1 + resolution + 1;  // beginning of next stack

                for(int j = 0; j < resolution; ++j, ++k1, ++k2)
                {
                    // 2 triangles per sector excluding first and last stacks
                    // k1 => k2 => k1+1
                    if(i != 0)
                    {
                        indices.push_back(k1);
                        indices.push_back(k2);
                        indices.push_back(k1 + 1);
                    }

                    // k1+1 => k2 => k2+1
                    if(i != (resolution - 1))
                    {
                        indices.push_back(k1 + 1);
                        indices.push_back(k2);
                        indices.push_back(k2 + 1);
                    }
                }
            }

            return CreateMeshData(texture, modelMatrix, vertices, indices);
        }

        MeshData* MeshBuilder::CreateCone(Texture* const texture, Matrix4* const modelMatrix, float height, float radius, int resolution)
        {
            if(resolution < 3)
            {
                Application::PrintLog(TypeMismatchErr, "Resolution can not be smaller then 3, value set to 3.");
                resolution = 3;
            }

            vector<RawVertex> vertices;
            vector<unsigned int> indices;

            float x, z;
            float yQuarter = (float) height / 4.0f;
            float sectorStep = 2.0f * PI / (float) resolution; // radian
            float currSectorAngle;

            Vector3 vec = Vector3(0, -yQuarter, 0);
            vertices.push_back(RawVertex(vec, vec * 255)); // bottom center

            vec = Vector3(0, yQuarter * 3, 0);
            vertices.push_back(RawVertex(vec, vec * 255)); // top center

            for(int i = 0; i < resolution; ++i)
            {
                currSectorAngle = i * sectorStep;

                x = radius * CosRad(currSectorAngle);
                z = radius * SinRad(currSectorAngle);

                vec = Vector3(x, -yQuarter, z);
                vertices.push_back(RawVertex(vec, vec * 255));
            }

            unsigned int topCenter = 1;
            unsigned int bottomCenter = 0;
            for(unsigned int i = 2; i < resolution + 2; i++)
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

            return CreateMeshData(texture, modelMatrix, vertices, indices);
        }

        MeshData* MeshBuilder::CreateSimit(Texture* const texture, Matrix4* const modelMatrix, float radius, float halfThickness, int resolution)
        {
            if(resolution < 3)
            {
                Application::PrintLog(TypeMismatchErr, "Resolution can not be smaller then 3, value set to 3.");
                resolution = 3;
            }

            vector<RawVertex> vertices;
            vector<unsigned int> indices;

            float x, y, z;
            float sectorSteps = 2 * PI / (float) resolution; // radian
            float currMajorAngle, currMinorAngle;

            for(int i = 0; i < resolution; ++i)
            {
                currMajorAngle = i * sectorSteps;

                for(int j = 0; j < resolution; j++)
                {
                    currMinorAngle = j * sectorSteps;
                    x = (radius + halfThickness * CosRad(currMinorAngle)) * CosRad(currMajorAngle);
                    y = halfThickness * SinRad(currMinorAngle);
                    z = (radius + halfThickness * CosRad(currMinorAngle)) * SinRad(currMajorAngle);

                    Vector3 vec = Vector3(x, y, z);
                    vertices.push_back(RawVertex(vec, vec * 255));
                }
            }

            for(int i = 0; i < resolution; i++) // major
            {
                int currMajorIndex = (i % resolution) * resolution;
                int nextMajorIndex = ((i + 1) % resolution) * resolution;

                for(int j = 0; j < resolution; j++) // minor
                {
                    indices.push_back(currMajorIndex + j);
                    indices.push_back(nextMajorIndex + j);
                    indices.push_back(currMajorIndex + (j + 1) % resolution);
                    indices.push_back(nextMajorIndex + j);
                    indices.push_back(nextMajorIndex + (j + 1) % resolution);
                    indices.push_back(currMajorIndex + (j + 1) % resolution);
                }
            }

            return CreateMeshData(texture, modelMatrix, vertices, indices);
        }

#pragma endregion

#pragma endregion

#pragma endregion
    }
}
