#pragma once
#include "Encin/Tools/MathYaman.h"

#include <GLAD/glad.h>
#include <GLFW/glfw3.h>

#include <typeindex>

namespace GamEncin
{
    namespace Toolkit
    {
#pragma region Other

#pragma region Enums

        // all the layers that can be used in the game
        enum Layer
        {
            Default, IgnoreRay, UI, Player, Layer0, Layer1, Layer2, Layer3, Layer4, Layer5, Layer6, Layer7, Layer8, Layer9
        };

        // all end types, exit codes that can be used in the game
        enum LogType
        {
            Safe, GLFWErr, GLADErr, ShaderCompilationErr, ShaderLinkingErr, ElementCouldNotFoundErr, TypeMismachErr, IOErr, IODeviceWarn, ProgramDuplicationErr, NullPointerErr, ElementDuplicationErr, IndexOutOfRangeErr, idoiterr, ComponentRequirementErr
        };

#pragma endregion

#pragma region BinaryTree

        //TODO
        //template <typename T>
        //struct BinaryTreeNode
        //{
        //    BinaryTreeNode(T data) : data(data) {}
        //
        //    T data;
        //    BinaryTreeNode* left = nullptr;
        //    BinaryTreeNode* right = nullptr;
        //};
        //
        //template <typename T>
        //struct BinaryTree
        //{
        //public:
        //    BinaryTree() = default;
        //
        //    BinaryTreeNode<T>* GetRoot() const { return root; }
        //
        //    void AddNode(T data);
        //    void DeleteNode(T data);
        //    void Clear();
        //
        //private:
        //    BinaryTreeNode<T>* root = nullptr;
        //};

#pragma endregion

#pragma endregion

#pragma region Texture

        struct Texture
        {
            unsigned int id = 0;
            unsigned int bitsPerPixel = 0;
            unsigned char* data = nullptr;
            unsigned long long handle = 0;
            Vector2Int size = Vector2Int::Zero();
            string filePath = "";

            Texture() = default;
            Texture(unsigned int id, unsigned int bitsPerPixel, unsigned char* data, unsigned long long handle, Vector2Int size, string filePath);

            void Initialize();
        };

        class TextureManager
        {
        public:
            static Texture* GetTexture(string textureFilePath);
            static void DeleteTexture(Texture* textureToDelete);
            static void InitializeTextures();

        private:
            static vector<Texture*> loadedTextures;
        };

#pragma endregion

#pragma region Text

        struct Font
        {
        public:
            string name = "";
            Texture* texture = nullptr;
            unordered_map<char, Vector2Int> charUVs;
            Vector2Int sizePerChar = Vector2Int::Zero();
            string bdfFilePath = "";

            Font(string name, string bdfFilePath, Texture* sourceImage);
        };

        class FontManager
        {
        public:
            /// <summary>
            /// Gets the .bdf font file from the given path and creates a font object.
            /// </summary>
            /// <param name="fontFilePath:">File path of the .bdf font file</param>
            /// <returns>Returns the already created font if the font in file path is already created. Otherwise creates the font and returns it.</returns>
            static Font* GetFont(string bdfFilePath);

            /// <summary>
            /// Deletes the font object.
            /// </summary>
            /// <param name="fontToDelete"></param>
            static void DeleteFont(Font* fontToDelete);

        private:
            static vector<Font*> loadedFonts;
        };

#pragma endregion

#pragma region Mesh Tools

        struct Edge;
        struct Face;

        struct RawVertex
        {
        public:
            unsigned int objectId = 0;

            Vector3 position = Vector3::Zero();
            Vector3 normal = Vector3::Forward();
            Vector2 uv = Vector2::Zero();
            //TODO order matters due to shader layout
            //TODO dont forget to link attributes

            RawVertex(Vector3 position, Vector2 texture);
            void SetObjectId(unsigned int objectId);
            void AddNormal(Vector3 normal);
            void NormalizeNormal();
        };

        struct Vertex : RawVertex
        {
        public:
            unsigned int id = 0;

            vector<Face*> faces;
            vector<Edge*> edges;

            Vertex(unsigned int id, const RawVertex& rawVertex);
            void AddFace(Face* face);
            void AddEdge(Edge* edge);
            Face* TryGetFace(unsigned int faceId);
            Edge* TryGetEdge(unsigned int edgeId);
        };

        struct Edge
        {
        public:
            unsigned int id = 0;

            Vertex* startVertex = nullptr;
            Vertex* endVertex = nullptr;

            Face* leftFace = nullptr;
            Face* rightFace = nullptr;

            Edge(Vertex* startVertex, Vertex* endVertex);
        };

        struct Face
        {
        public:
            unsigned int id = 0;

            array<Vertex*, 3> vertices = {nullptr, nullptr, nullptr};
            array<Edge*, 3> edges = {nullptr, nullptr, nullptr};

            Face(Vertex* vertex1, Vertex* vertex2, Vertex* vertex3);
            void SetEdges(Edge* edge1, Edge* edge2, Edge* edge3);
            bool IsEdgeDirectionMatchingFace(Edge* edge);
        };

        struct MeshData
        {
            //TODO make getters and setters
        public:
            unsigned int id = 0;
            unsigned int batchVertexOffset = 0;
            unsigned int batchIndexOffset = 0;

            vector<Vertex*> vertices;
            unordered_map<unsigned int, Edge*> edges;
            vector<Face*> faces;

            MeshData(vector<Vertex*> vertices, unordered_map<unsigned int, Edge*> edges, vector<Face*> faces);

            vector<unsigned int> GetModelIndexArray();
            vector<RawVertex> GetModelRawVertexArray();

            void SetForBatch(unsigned int id, unsigned int batchVertexOffset, unsigned int batchIndexOffset);

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
