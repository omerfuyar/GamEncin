#include "Encin/Encin.h"



namespace GamEncin
{
    namespace Toolkit
    {
#pragma region Mesh Tools

#pragma region Primitives

        RawVertex::RawVertex(Vector3 position, Vector4 color)
        {
            this->position = position;
            this->color = color;
        }

        RawVertex::RawVertex(Vector3 position, Vector4 color, Vector3 normal, Vector2 texture)
        {
            this->position = position;
            this->color = color;
            //this->normal = normal; TODO
            //this->texture = texture;
        }

        void RawVertex::SetObjectId(unsigned int objectId)
        {
            this->objectId = objectId;
        }

        void RawVertex::SetPosition(Vector3 position)
        {
            this->position = position;
        }

        void RawVertex::SetColor(Vector4 color)
        {
            this->color = color;
        }

        void RawVertex::SetNormal(Vector3 normal)
        {
            // this->normal = normal; TODO
        }

        void RawVertex::SetTexture(Vector2 texture)
        {
            //this->texture = texture; TODO
        }

        Vertex::Vertex(unsigned int id, const RawVertex& rawVertex) : RawVertex(rawVertex)
        {
            this->id = id;
        }

        void Vertex::AddFace(Face* face)
        {
            faces[face->id] = face;
        }

        void Vertex::AddEdge(Edge* edge)
        {
            edges[edge->id] = edge;
        }

        Edge::Edge(Vertex* startVertex, Vertex* endVertex)
        {
            id = MeshBuilder::GenerateEdgeId(startVertex, endVertex);

            this->startVertex = startVertex;
            this->endVertex = endVertex;

            startVertex->AddEdge(this);
            endVertex->AddEdge(this);
        }

        Face::Face(Vertex* vertex1, Vertex* vertex2, Vertex* vertex3)
        {
            id = MeshBuilder::GenerateFaceId(vertex1, vertex2, vertex3);

            vertices[0] = vertex1;
            vertices[1] = vertex2;
            vertices[2] = vertex3;

            vertex1->AddFace(this);
            vertex2->AddFace(this);
            vertex3->AddFace(this);
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

        MeshData::MeshData(unsigned int vertexCount)
        {
            vertices.resize(vertexCount);
        }

        vector<unsigned int> MeshData::GetIndiceArray()
        {
            vector<unsigned int> indices;

            for(Face* face : faces)
            {
                for(int i = 0; i < 3; i++)
                {
                    indices.push_back(face->vertices[i]->id);
                }
            }

            return indices;
        }

        vector<RawVertex> MeshData::GetRawVertexArray()
        {
            vector<RawVertex> rawVertices;

            for(RawVertex* rawVert : vertices)
            {
                rawVertices.push_back(*rawVert);
            }

            return rawVertices;
        }

        Edge* MeshData::TryFindEdge(unsigned int edgeId)
        {
            for(Edge* edge : edges)
            {
                if(edge && edge->id == edgeId)
                {
                    return edge;
                }
            }

            return nullptr;
        }

        void MeshData::DeleteData()
        {
            for(Vertex* vert : vertices)
            {
                delete vert;
            }
            vertices.clear();

            for(Edge* edge : edges)
            {
                delete edge;
            }
            edges.clear();

            for(Face* face : faces)
            {
                delete face;
            }
            faces.clear();
        }

#pragma endregion

#pragma region MeshBuilder

        MeshData* MeshBuilder::CreateMeshData(const vector<RawVertex> vertices, const vector<unsigned int> indices)
        {
            MeshData* meshData = new MeshData(vertices.size());

            int vertexCount = vertices.size();
            for(int i = 0; i < vertexCount; i++)
            {
                meshData->vertices[i] = new Vertex(i, vertices[i]);
            }

            int triangleCount = indices.size() / 3;
            for(int i = 0; i < triangleCount; i++) //for triangles
            {
                Vertex* faceVertices[3] = {meshData->vertices[indices[i * 3]],
                                           meshData->vertices[indices[i * 3 + 1]],
                                           meshData->vertices[indices[i * 3 + 2]]};

                Face* face = new Face(faceVertices[0], faceVertices[1], faceVertices[2]);
                //edges of face is empty

                for(int i = 0; i < 3; i++) //for face edges
                {
                    Vertex* startVert = faceVertices[i];
                    Vertex* endVert = faceVertices[(i + 1) % 3];

                    unsigned int edgeId = MeshBuilder::GenerateEdgeId(startVert, endVert);
                    Edge* edge = meshData->TryFindEdge(edgeId);

                    if(edge)
                    {
                        edge->rightFace = face;
                    }
                    else //if null
                    {
                        edge = new Edge(startVert, endVert);
                        edge->leftFace = face;
                        meshData->edges.push_back(edge);
                    }

                    face->edges[i] = edge;
                }
                //edges of face and meshData is set

                meshData->faces.push_back(face);
                //face of meshData is set
            }

            //printf("MeshBuilder::CreateMeshData: %u vertices, %u edges, %u faces\n",
            //       (unsigned int) meshData->vertices.size(),
            //       (unsigned int) meshData->edges.size(),
            //       (unsigned int) meshData->faces.size());
            //
            //printf("total size of meshData: ");
            //unsigned long int size = 0;
            //
            //size += meshData->vertices.size() * sizeof(Vertex);
            //size += meshData->edges.size() * sizeof(Edge);
            //size += meshData->faces.size() * sizeof(Face);
            //
            //printf("%lu bytes\n\n", size);
            //
            //for(Edge* edge : meshData->edges)
            //{
            //    if(!edge->leftFace)
            //    {
            //        printf("Edge %u : %u, %u : left face didn't attached (boundary?)\n", edge->id, edge->startVertex->id, edge-//>endVertex->id);
            //
            //    }
            //    if(!edge->rightFace)
            //    {
            //        printf("Edge %u : %u, %u : right face didn't attached (boundary?)\n", edge->id, edge->startVertex->id, edge-//>endVertex->id);
            //    }
            //}

            return meshData;
        }

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
                RawVertex(Vector3(-1, -1, -1) * xyzCoord, Vector3(128, 0, 128)),
                RawVertex(Vector3(1, -1, -1) * xyzCoord, Vector3(255, 165, 0)),
                RawVertex(Vector3(1, -1, 1) * xyzCoord, Vector3(0, 128, 128)),
                RawVertex(Vector3(-1, -1, 1) * xyzCoord, Vector3(75, 0, 130)),
                RawVertex(Vector3(-1, 1, 1) * xyzCoord, Vector3(255, 20, 147)),
                RawVertex(Vector3(1, 1, 1) * xyzCoord, Vector3(0, 255, 127)),
                RawVertex(Vector3(1, 1, -1) * xyzCoord, Vector3(255, 105, 180)),
                RawVertex(Vector3(-1, 1, -1) * xyzCoord, Vector3(0, 191, 255))
            };

            vector<unsigned int> indices = {
                0, 1, 2, 2, 3, 0,
                4, 5, 6, 6, 7, 4,
                1, 2, 5, 5, 6, 1,
                0, 1, 6, 6, 7, 0,
                2, 3, 4, 4, 5, 2,
                3, 0, 7, 7, 4, 3
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



#pragma endregion

#pragma endregion
    }
}
