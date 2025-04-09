#pragma once
#include "Encin/Tools/MathYaman.h"

#include <GLAD/glad.h>

#include <Windows.h>

#define GE_SELECT_OPTIMUM_GPU extern"C"{_declspec(dllexport)DWORD NvOptimusEnablement=0x00000001;_declspec(dllexport)DWORD AmdPowerXpressRequestHighPerformance=0x00000001;}

namespace GamEncin
{
    namespace Toolkit
    {
#pragma region Enums

        // all the layers that can be used in the game
        enum Layer
        {
            Default, IgnoreRay, UI, Player, Layer0, Layer1, Layer2, Layer3, Layer4, Layer5, Layer6, Layer7, Layer8, Layer9
        };

        // all end types, exit codes that can be used in the game
        enum LogType
        {
            Safe, GLFWErr, GLADErr, ShaderCompilationErr, ShaderLinkingErr, ElementCouldNotFoundErr, TypeMismachErr, IOErr, IODeviceWarn, ProgramDuplicationErr, NullPointerErr, ElementDuplicationErr
        };

#pragma endregion

#pragma region Mesh Tools

        struct Edge;
        struct Face;

        struct RawVertex
        {
            Vector3 position = Vector3::Zero();
            //Vector3 normal = Vector3::Forward();

            //Vector2 texture = Vector2::Zero();

            Vector4 color = Vector4::One();

            RawVertex(Vector3 position, Vector4 color);
            RawVertex(Vector3 position, Vector4 color, Vector3 normal, Vector2 texture);
            void SetPosition(Vector3 position);
            void SetNormal(Vector3 normal);
            void SetTexture(Vector2 texture);
            void SetColor(Vector4 color);
        };

        struct Vertex : RawVertex
        {
            unsigned int id = 0;

            unordered_map<unsigned int, Face*> faces;
            unordered_map<unsigned int, Edge*> edges;

            Vertex(unsigned int id, const RawVertex& rawVertex);
            void AddFace(Face* face);
            void AddEdge(Edge* edge);
        };

        struct Edge
        {
            unsigned int id = 0;

            Vertex* startVertex = nullptr;
            Vertex* endVertex = nullptr;

            Face* leftFace = nullptr;
            Face* rightFace = nullptr;

            Edge(Vertex* startVertex, Vertex* endVertex);
        };

        struct Face
        {
            unsigned int id = 0;

            array<Vertex*, 3> vertices = {nullptr, nullptr, nullptr};
            array<Edge*, 3> edges = {nullptr, nullptr, nullptr};

            Face(Vertex* vertex1, Vertex* vertex2, Vertex* vertex3);
            void SetEdges(Edge* edge1, Edge* edge2, Edge* edge3);
            bool IsEdgeDirectionMatchingFace(Edge* edge);
        };

        struct MeshData
        {
            unordered_map<unsigned int, Vertex*> vertices;
            unordered_map<unsigned int, Edge*> edges;
            unordered_map<unsigned int, Face*> faces;

            vector<unsigned int> GetIndiceArray();
            vector<RawVertex> GetRawVertexArray();

            void DeleteData();
        };

        class MeshBuilder
        {
        private:
            MeshBuilder() = delete;
            MeshBuilder(const MeshBuilder&) = delete;
            void operator = (const MeshBuilder&) = delete;

        public:
            static MeshData* CreateMeshData(const vector<RawVertex> vertices, const vector<unsigned int> indices);

            static MeshData* CreateCube(float sideLength = 1.0f);
            static MeshData* CreatePlane(float sideLength = 1.0f);
            static MeshData* CreateCircle(float halfRadius = 0.5f, int resolution = 20);
            static MeshData* CreateCylinder(float height = 1.0f, float halfRadius = 0.5f, int resolution = 20);
            static MeshData* CreateSphere(float halfRadius = 0.5f, int resolution = 20);
            static MeshData* CreatePyramid(float height = 1.0f, float baseLength = 1.0f);
            static MeshData* CreateCone(float height = 1.0f, float halfRadius = 0.5f, int resolution = 20);
            static MeshData* CreateSimit(float halfRadius = 0.5f, float halfThickness = 0.25f, int resolution = 20);
            //works until 2^10 vertices
            static unsigned int GenerateFaceId(Vertex* vertex1, Vertex* vertex2, Vertex* vertex3);
            //works until 2^16 vertices
            static unsigned int GenerateEdgeId(Vertex* vertex1, Vertex* vertex2);
        };

#pragma endregion
    }
}

using namespace GamEncin::Toolkit;
