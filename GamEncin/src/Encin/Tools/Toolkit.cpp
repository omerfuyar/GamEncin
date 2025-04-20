#include "Encin/Encin.h"
#define STB_IMAGE_IMPLEMENTATION
#include <STB/stb_image.h>


namespace GamEncin
{
    namespace Toolkit
    {
#pragma region Textures

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

            unsigned int imageFormat = (bitsPerPixel / 8 == 4) ? GL_RGBA :
                (bitsPerPixel / 8 == 3) ? GL_RGB :
                (bitsPerPixel / 8 == 2) ? GL_RG : GL_RED;

            glTexImage2D(GL_TEXTURE_2D, 0, imageFormat, size.x, size.y, 0, imageFormat, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

            handle = glGetTextureHandleARB(id);
            glMakeTextureHandleResidentARB(handle);

            stbi_image_free(data);

            printf("Texture %s loaded with id: %u\n", filePath.c_str(), id);
            printf("Texture %s handle: %llu\n", filePath.c_str(), handle);
            printf("Texture %s size: %d x %d\n", filePath.c_str(), size.x, size.y);
            printf("Texture %s bits per pixel: %u\n", filePath.c_str(), bitsPerPixel);
            printf("Texture %s data size: %d\n", filePath.c_str(), size.x * size.y * (bitsPerPixel / 8));
        }

        void Texture::Delete()
        {
            glMakeTextureHandleNonResidentARB(handle);
            glDeleteTextures(1, &id);
        }

        vector<Texture*> TextureManager::loadedTextures;

        Texture* TextureManager::GetTexture(string textureFilePath)
        {
            printf("size : %d\n", loadedTextures.size());

            auto obj = std::find_if(loadedTextures.begin(), loadedTextures.end(), [&textureFilePath](Texture* texture)
                                    {
                                        return texture->filePath == textureFilePath;
                                    });

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

            if(Application::isRunning)
            {
                texture->Initialize();
            }

            loadedTextures.push_back(texture);

            return texture;
        }

        void TextureManager::AddTexture(Texture* texture)
        {
            if(!texture)
            {
                Application::PrintLog(NullPointerErr, "Texture trying to add is null");
                return;
            }

            auto obj = std::find(loadedTextures.begin(), loadedTextures.end(), texture);

            if(obj != loadedTextures.end())
            {
                Application::PrintLog(ElementDuplicationErr, "Texture already exists in the renderer");
                return;
            }

            loadedTextures.push_back(texture);
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
                Application::PrintLog(ElementCouldNotFoundErr, "Couldn't found texture to delete");
                return;
            }

            textureToDelete->Delete();

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

#pragma region Mesh Tools

#pragma region Primitives

#pragma region RawVertex

        RawVertex::RawVertex(Vector3 position, Vector4 temp)
        {
            this->position = position;
            this->uv = Vector2(temp.x, temp.y);
        }

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

        MeshData::MeshData(vector<Vertex*> vertices, unordered_map<unsigned int, Edge*> edges, vector<Face*> faces)
        {
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

        vector<unsigned int> MeshData::GetIndiceArray()
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

        vector<RawVertex> MeshData::GetRawVertexArray()
        {
            vector<RawVertex> rawVertices;

            for(RawVertex* rawVertex : vertices)
            {
                rawVertices.push_back(*rawVertex);
            }

            return rawVertices;
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

        MeshData* MeshBuilder::CreateMeshData(const vector<RawVertex> vertices, const vector<unsigned int> indices)
        {
            vector<Vertex*> tempVertices;
            tempVertices.resize(vertices.size());
            unordered_map<unsigned int, Edge*> tempEdges;
            vector<Face*> tempFaces;

            int vertexCount = vertices.size();
            for(int i = 0; i < vertexCount; i++)
            {
                tempVertices[i] = new Vertex(i, vertices[i]);
            }

            int triangleCount = indices.size() / 3;
            for(int i = 0; i < triangleCount; i++) //for triangles
            {
                Vertex* faceVertices[3] = {tempVertices[indices[i * 3]],
                                           tempVertices[indices[i * 3 + 1]],
                                           tempVertices[indices[i * 3 + 2]]};


                Face* face = new Face(faceVertices[0], faceVertices[1], faceVertices[2]);
                //edges of face is empty

                for(int i = 0; i < 3; i++) //for face edges
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
                    else //if null
                    {
                        edge = new Edge(startVert, endVert);
                        edge->leftFace = face;
                        tempEdges[edgeId] = edge;
                    }

                    face->edges[i] = edge;
                }
                //edges of face and meshData is set

                tempFaces.push_back(face);
                //face of meshData is set
            }

            for(Vertex* vertex : tempVertices)
            {
                vertex->NormalizeNormal();
            }

            return new MeshData(tempVertices, tempEdges, tempFaces);
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

        MeshData* MeshBuilder::CreatePlane(float sideLength)
        {
            float xzCoord = sideLength / 2;

            vector<RawVertex> vertices =
            {
                RawVertex(Vector3(1, 0, 1) * xzCoord, Vector3(1, 0, 1) * xzCoord * 255),
                RawVertex(Vector3(1, 0, -1) * xzCoord, Vector3(1, 0, -1) * xzCoord * 255),
                RawVertex(Vector3(-1, 0, -1) * xzCoord, Vector3(-1, 0, -1) * xzCoord * 255),
                RawVertex(Vector3(-1, 0, 1) * xzCoord, Vector3(-1, 0, 1) * xzCoord * 255)
            };

            vector<unsigned int> indices = {0, 1, 2, 2, 3, 0};

            return CreateMeshData(vertices, indices);
        }

        MeshData* MeshBuilder::CreateCircle(float halfRadius, int resolution)
        {
            if(resolution < 3)
            {
                Application::PrintLog(TypeMismachErr, "Resolution can not be smaller then 3, value set to 3.");
                resolution = 3;
            }

            vector<RawVertex> vertices;
            vector<unsigned int> indices;

            float x, z;
            float sectorStep = 2 * PI / resolution; //radian
            float currSectorAngle;

            vertices.push_back(RawVertex(Vector3(0, 0, 0), Vector3(0, 0, 0) * 255));

            for(int i = 0; i < resolution; ++i)
            {
                currSectorAngle = i * sectorStep;

                x = halfRadius * CosRad(currSectorAngle);
                z = halfRadius * SinRad(currSectorAngle);

                Vector3 vec = Vector3(x, 0, z);
                vertices.push_back(RawVertex(vec, vec * 255));
            }

            for(int i = 0; i < resolution; i++)
            {
                indices.push_back(0); //center
                indices.push_back(i + 1);
                indices.push_back((i + 1) % resolution + 1);
            }

            return CreateMeshData(vertices, indices);
        }

        MeshData* MeshBuilder::CreateCube(float sideLength)
        {
            float xyzCoord = sideLength / 2;

            vector<RawVertex> vertices =
            {
                // Bottom face
                RawVertex(Vector3(-1, -1, -1) * xyzCoord, Vector2(0, 0)),
                RawVertex(Vector3(1, -1, -1) * xyzCoord, Vector2(1, 0)),
                RawVertex(Vector3(1, -1, 1) * xyzCoord, Vector2(1, 1)),
                RawVertex(Vector3(-1, -1, 1) * xyzCoord, Vector2(0, 1)),

                // Top face
                RawVertex(Vector3(-1, 1, -1) * xyzCoord, Vector2(0, 0)),
                RawVertex(Vector3(1, 1, -1) * xyzCoord, Vector2(1, 0)),
                RawVertex(Vector3(1, 1, 1) * xyzCoord, Vector2(1, 1)),
                RawVertex(Vector3(-1, 1, 1) * xyzCoord, Vector2(0, 1)),

                // Front face
                RawVertex(Vector3(-1, -1, 1) * xyzCoord, Vector2(0, 0)),
                RawVertex(Vector3(1, -1, 1) * xyzCoord, Vector2(1, 0)),
                RawVertex(Vector3(1, 1, 1) * xyzCoord, Vector2(1, 1)),
                RawVertex(Vector3(-1, 1, 1) * xyzCoord, Vector2(0, 1)),

                // Back face
                RawVertex(Vector3(-1, -1, -1) * xyzCoord, Vector2(0, 0)),
                RawVertex(Vector3(1, -1, -1) * xyzCoord, Vector2(1, 0)),
                RawVertex(Vector3(1, 1, -1) * xyzCoord, Vector2(1, 1)),
                RawVertex(Vector3(-1, 1, -1) * xyzCoord, Vector2(0, 1)),

                // Left face
                RawVertex(Vector3(-1, -1, -1) * xyzCoord, Vector2(0, 0)),
                RawVertex(Vector3(-1, -1, 1) * xyzCoord, Vector2(1, 0)),
                RawVertex(Vector3(-1, 1, 1) * xyzCoord, Vector2(1, 1)),
                RawVertex(Vector3(-1, 1, -1) * xyzCoord, Vector2(0, 1)),

                // Right face
                RawVertex(Vector3(1, -1, -1) * xyzCoord, Vector2(0, 0)),
                RawVertex(Vector3(1, -1, 1) * xyzCoord, Vector2(1, 0)),
                RawVertex(Vector3(1, 1, 1) * xyzCoord, Vector2(1, 1)),
                RawVertex(Vector3(1, 1, -1) * xyzCoord, Vector2(0, 1)),
            };

            vector<unsigned int> indices = {
                // Bottom face
                0, 1, 2, 2, 3, 0,
                // Top face
                4, 5, 6, 6, 7, 4,
                // Front face
                8, 9, 10, 10, 11, 8,
                // Back face
                12, 13, 14, 14, 15, 12,
                // Left face
                16, 17, 18, 18, 19, 16,
                // Right face
                20, 21, 22, 22, 23, 20
            };

            return CreateMeshData(vertices, indices);
        }

        MeshData* MeshBuilder::CreatePyramid(float height, float baseLength)
        {
            float xzCoord = baseLength / 2;
            float yQuarter = height / 4;

            vector<RawVertex> vertices =
            {
                RawVertex(Vector3(0.0, yQuarter * 3, 0.0), Vector3(255, 0, 0)),
                RawVertex(Vector3(-xzCoord, -yQuarter, xzCoord), Vector3(0, 255, 0)),
                RawVertex(Vector3(xzCoord, -yQuarter, xzCoord), Vector3(0, 0, 255)),
                RawVertex(Vector3(xzCoord, -yQuarter, -xzCoord), Vector3(255, 255, 0)),
                RawVertex(Vector3(-xzCoord, -yQuarter, -xzCoord), Vector3(0, 255, 255))
            };

            vector<unsigned int> indices = {
                0, 1, 2, 0, 2, 3,
                0, 3, 4, 0, 4, 1,
                1, 2, 3, 3, 4, 1
            };

            return CreateMeshData(vertices, indices);
        }

        MeshData* MeshBuilder::CreateCylinder(float height, float halfRadius, int resolution)
        {
            if(resolution < 3)
            {
                Application::PrintLog(TypeMismachErr, "Resolution can not be smaller then 3, value set to 3.");
                resolution = 3;
            }

            vector<RawVertex> vertices;
            vector<unsigned int> indices;

            float x, y, z;
            float sectorStep = 2 * PI / resolution; //radian
            float currSectorAngle;

            for(int i = -1; i < 2; i += 2)
            {
                y = i * height / 2;
                vertices.push_back(RawVertex(Vector3(0, y, 0), Vector3(0, y, 0) * 255));

                for(int j = 0; j < resolution; ++j)
                {
                    currSectorAngle = j * sectorStep;

                    x = halfRadius * CosRad(currSectorAngle);
                    z = halfRadius * SinRad(currSectorAngle);

                    Vector3 vec = Vector3(x, y, z);

                    vertices.push_back(RawVertex(vec, vec * 255));
                }
            }

            int center2 = resolution + 1;
            for(int i = 0; i < resolution; ++i)
            {
                //circles
                indices.push_back(0); //center1
                indices.push_back(i + 1);
                indices.push_back((i + 1) % resolution + 1);
                indices.push_back(center2); //center2
                indices.push_back(center2 + i + 1);
                indices.push_back(center2 + (i + 1) % resolution + 1);

                //sides
                indices.push_back(i + 1);
                indices.push_back((i + 1) % resolution + 1);
                indices.push_back(i + resolution + 2);
                indices.push_back((i + 1) % resolution + 1);
                indices.push_back(i + resolution + 2);
                indices.push_back((i + 1) % resolution + resolution + 2);
            }

            return CreateMeshData(vertices, indices);
        }

        MeshData* MeshBuilder::CreateSphere(float halfRadius, int resolution)
        {
            if(resolution < 3)
            {
                Application::PrintLog(TypeMismachErr, "Resolution can not be smaller then 3, value set to 3.");
                resolution = 3;
            }

            vector<RawVertex> vertices;
            vector<unsigned int> indices;

            //TODO this is not compatible with shaders and normals
            //this sphere is rendering from top to bottom,

            float x, y, z, xz;
            float stackStep = PI / resolution; //radian
            float sectorStep = 2 * PI / resolution; //radian
            float currStackAngle, currSectorAngle;

            for(int i = 0; i <= resolution; ++i)
            {
                currStackAngle = PI / 2 - i * stackStep; //from pi/2 to -pi/2 inclusive

                xz = halfRadius * CosRad(currStackAngle);
                y = halfRadius * SinRad(currStackAngle);

                for(int j = 0; j <= resolution; ++j)
                {
                    currSectorAngle = j * sectorStep; //from 0 to 2pi inclusive

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
                k1 = i * (resolution + 1);     // beginning of current stack
                k2 = k1 + resolution + 1;      // beginning of next stack

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

            return CreateMeshData(vertices, indices);
        }

        MeshData* MeshBuilder::CreateCone(float height, float halfRadius, int resolution)
        {
            if(resolution < 3)
            {
                Application::PrintLog(TypeMismachErr, "Resolution can not be smaller then 3, value set to 3.");
                resolution = 3;
            }

            vector<RawVertex> vertices;
            vector<unsigned int> indices;

            float x, z;
            float yQuarter = (float) height / 4.0f;
            float sectorStep = 2.0f * PI / (float) resolution; //radian
            float currSectorAngle;

            Vector3 vec = Vector3(0, -yQuarter, 0);
            vertices.push_back(RawVertex(vec, vec * 255)); //bottom center

            vec = Vector3(0, yQuarter * 3, 0);
            vertices.push_back(RawVertex(vec, vec * 255)); //top center

            for(int i = 0; i < resolution; ++i)
            {
                currSectorAngle = i * sectorStep;

                x = halfRadius * CosRad(currSectorAngle);
                z = halfRadius * SinRad(currSectorAngle);

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

            return CreateMeshData(vertices, indices);
        }

        MeshData* MeshBuilder::CreateSimit(float halfRadius, float halfThickness, int resolution)
        {
            if(resolution < 3)
            {
                Application::PrintLog(TypeMismachErr, "Resolution can not be smaller then 3, value set to 3.");
                resolution = 3;
            }

            vector<RawVertex> vertices;
            vector<unsigned int> indices;

            float x, y, z;
            float sectorSteps = 2 * PI / (float) resolution; //radian
            float currMajorAngle, currMinorAngle;

            for(int i = 0; i < resolution; ++i)
            {
                currMajorAngle = i * sectorSteps;

                for(int j = 0; j < resolution; j++)
                {
                    currMinorAngle = j * sectorSteps;
                    x = (halfRadius + halfThickness * CosRad(currMinorAngle)) * CosRad(currMajorAngle);
                    y = halfThickness * SinRad(currMinorAngle);
                    z = (halfRadius + halfThickness * CosRad(currMinorAngle)) * SinRad(currMajorAngle);


                    Vector3 vec = Vector3(x, y, z);
                    vertices.push_back(RawVertex(vec, vec * 255));
                }
            }

            for(int i = 0; i < resolution; i++) //major
            {
                int currMajorIndex = (i % resolution) * resolution;
                int nextMajorIndex = ((i + 1) % resolution) * resolution;

                for(int j = 0; j < resolution; j++) //minor
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
    }
}
