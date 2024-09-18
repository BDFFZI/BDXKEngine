#pragma once
#include <vector>
#include "Core/Runtime/Base/Data/Math/Vector/Vector3.h"
#include "Core/Runtime/Base/Reflection/Reflection.h"
#include "Core/Runtime/Platform/GL/Buffer/Buffer.h"
#include "Core/Runtime/Platform/GL/Shader/Shader.h"

namespace BDXKEngine
{
    class Mesh : public Object
    {
    public:
        static ObjectPtr<Mesh> Create();

        int GetVerticesCount() const;
        int GetTrianglesCount() const;
        std::vector<unsigned short> GetTriangles();
        std::vector<Vector3> GetPositions() const;
        std::vector<Vector3> GetNormals() const;
        std::vector<Vector4> GetTangents() const;
        std::vector<Vector2> GetUVs() const;
        std::vector<Color> GetColors() const;
        std::vector<Vertex> GetVertices() const;

        void SetTriangles(const std::vector<unsigned short>& data);
        void SetPositions(const std::vector<Vector3>& data);
        void SetNormals(const std::vector<Vector3>& data);
        void SetTangents(const std::vector<Vector4>& data);
        void SetUVs(const std::vector<Vector2>& data);
        void SetColors(const std::vector<Color>& data);
        void SetVertices(const std::vector<Vertex>& data);
        void UpdateGL() const;
        // 设置顶点索引缓冲区
        void UploadRP() const;

    private:
        std::vector<Vertex> vertices = {};
        std::vector<unsigned short> triangles = {};

        ObjectPtr<Buffer> vertexBuffer;
        ObjectPtr<Buffer> triangleBuffer;

        void ResetVerticesBuffer();
        void ResetTrianglesBuffer();

        void Transfer(Transferer& transferer) override;
        void Awake() override;
    };

    CustomReflection(Mesh)
}
