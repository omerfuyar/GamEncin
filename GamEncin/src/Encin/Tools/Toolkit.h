#pragma once
#include "Encin/Tools/MathYaman.h"

#include <GLAD/glad.h>
#include <GLFW/glfw3.h>

#include <typeindex>

using std::to_string;

namespace GamEncin
{
    namespace Toolkit
    {
#pragma region Other

#pragma region Enums

        // all the layers that can be used in the game
        enum Layer
        {
            Default,
            IgnoreRay,
            UI,
            Player,
            Layer0,
            Layer1,
            Layer2,
            Layer3,
            Layer4,
            Layer5,
            Layer6,
            Layer7,
            Layer8,
            Layer9
        };

        // all end types, exit codes that can be used in the game
        enum LogType
        {
            Safe,
            GLFWErr,
            GLADErr,
            ShaderCompilationErr,
            ShaderLinkingErr,
            ElementCouldNotFindErr,
            TypeMismatchErr,
            IOErr,
            IODeviceWarn,
            ProgramDuplicationErr,
            NullPointerErr,
            ElementDuplicationErr,
            IndexOutOfRangeErr,
            idoiterr,
            ComponentRequirementErr
        };

#pragma endregion

#pragma endregion

#pragma region Texture

        struct Texture
        {
            unsigned int id = 0;
            unsigned int bitsPerPixel = 0;
            unsigned char *data = nullptr;
            unsigned long long handle = 0;
            Vector2Int size = Vector2Int::Zero();
            string sourceFilePath = "";
            int wrapModeS = GL_REPEAT;
            int wrapModeT = GL_REPEAT;
            int minFilter = GL_LINEAR_MIPMAP_LINEAR;
            int magFilter = GL_LINEAR;

            Texture() = default;
            Texture(unsigned int id, unsigned int bitsPerPixel, unsigned char *data, unsigned long long handle, Vector2Int size, string sourceFilePath);

            void SetWrapAndFilter(int wrapModeS, int wrapModeT, int minFilter, int magFilter);
            void Initialize();
        };

        class TextureManager
        {
        public:
            static Texture *GetTexture(string textureFilePath);
            static void DeleteTexture(Texture *textureToDelete);
            static void InitializeTextures();

        private:
            static vector<Texture *> loadedTextures;
        };

#pragma endregion

#pragma region TextMesh

        struct Character
        {
            char character = '?';
            Vector2 uv = Vector2::Zero();
            Vector2 size = Vector2::Zero();
            Vector2 offset = Vector2::Zero();

            Character() = default;
            Character(char character, Vector2 uv, Vector2 size, Vector2 offset);
        };

        struct Font
        {
        public:
            string name = "";
            Texture *texture = nullptr;
            unordered_map<char, Character> chars;
            string bdfFilePath = "";

            Font(string name, Texture *sourceImage, unordered_map<char, Character> chars, string bdfFilePath);
        };

        class FontManager
        {
        public:
            /// <summary>
            /// Gets the .bdf font file from the given path and creates a font object.
            /// </summary>
            /// <param name="fontFilePath:">File path of the .bdf font file</param>
            /// <returns>The already created font if the font in the file path is already created. Otherwise creates the font and returns it.</returns>
            static Font *GetFont(string bdfFilePath);

            /// <summary>
            /// Deletes the font object.
            /// </summary>
            /// <param name="fontToDelete"></param>
            static void DeleteFont(Font *fontToDelete);

            /// <summary>
            /// Creates a font object and a texture image from given .bdf file.
            /// </summary>
            /// <param name="bdfFilePath:">.bdf font file to import</param>
            /// <returns>The created ready to use font object.</returns>
            static Font *const CreateFontFromBDF(string bdfFilePath);

            /// <summary>
            /// Sets the atlas size of the font texture importer. Affects font atlases.
            /// </summary>
            /// <param name="atlasSize"></param>
            static void SetAtlasSize(Vector2Int atlasSize);

        private:
            static vector<Font *> loadedFonts;
            static Vector2Int atlasSize;
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
            // TODO order matters due to shader layout
            // TODO dont forget to link attributes

            RawVertex(Vector3 position, Vector2 texture);
            void SetObjectId(unsigned int objectId);
            void AddNormal(Vector3 normal);
            void NormalizeNormal();
        };

        struct Vertex : RawVertex
        {
        public:
            unsigned int id = 0;

            vector<Face *> faces;
            vector<Edge *> edges;

            Vertex(unsigned int id, const RawVertex &rawVertex);
            void AddFace(Face *face);
            void AddEdge(Edge *edge);
            Face *TryGetFace(unsigned int faceId);
            Edge *TryGetEdge(unsigned int edgeId);
        };

        struct Edge
        {
        public:
            unsigned int id = 0;

            Vertex *startVertex = nullptr;
            Vertex *endVertex = nullptr;

            Face *leftFace = nullptr;
            Face *rightFace = nullptr;

            Edge(Vertex *startVertex, Vertex *endVertex);
        };

        struct Face
        {
        public:
            unsigned int id = 0;

            array<Vertex *, 3> vertices = {nullptr, nullptr, nullptr};
            array<Edge *, 3> edges = {nullptr, nullptr, nullptr};

            Face(Vertex *vertex1, Vertex *vertex2, Vertex *vertex3);
            void SetEdges(Edge *edge1, Edge *edge2, Edge *edge3);
            bool IsEdgeDirectionMatchingFace(Edge *edge);
        };

        struct MeshData
        {
        public:
            MeshData() = default;
            MeshData(vector<Vertex *> vertices, unordered_map<unsigned int, Edge *> edges, vector<Face *> faces);

            unsigned int id = 0;
            unsigned int batchVertexOffset = 0;
            unsigned int batchIndexOffset = 0;

            vector<Vertex *> vertices;
            unordered_map<unsigned int, Edge *> edges;
            vector<Face *> faces;

            vector<RawVertex> GetRawVertexArray();
            vector<unsigned int> GetIndexArray();

            void SetForBatch(unsigned int id, unsigned int batchVertexOffset, unsigned int batchIndexOffset);
            void DeleteData();
        };

        class MeshBuilder
        {
        private:
            MeshBuilder() = delete;
            MeshBuilder(const MeshBuilder &) = delete;
            void operator=(const MeshBuilder &) = delete;

        public:
            static MeshData *CreateMeshData(const vector<RawVertex> vertices, const vector<unsigned int> indices);

            static MeshData *CreateCuboid(Vector3 size = Vector3::One());
            static MeshData *CreatePlane(Vector2 size = Vector2::One());
            static MeshData *CreateCircle(float radius = 0.5f, int resolution = 20);
            static MeshData *CreateCylinder(float height = 1.0f, float radius = 0.5f, int resolution = 20);
            static MeshData *CreateSphere(float radius = 0.5f, int resolution = 20);
            static MeshData *CreatePyramid(float height = 1.0f, float baseLength = 1.0f);
            static MeshData *CreateCone(float height = 1.0f, float radius = 0.5f, int resolution = 20);
            static MeshData *CreateSimit(float radius = 0.5f, float halfThickness = 0.25f, int resolution = 20);
            // works until 2^10 vertices
            static unsigned int GenerateFaceId(Vertex *vertex1, Vertex *vertex2, Vertex *vertex3);
            // works until 2^16 vertices
            static unsigned int GenerateEdgeId(Vertex *vertex1, Vertex *vertex2);
        };

#pragma endregion
    }
}

using namespace GamEncin::Toolkit;
