#pragma once
#include <vector>
#include "BDXKEngine/Base/Data/Mathematics/Vector/Vector3.h"
#include "../GL.h"
#include "../GLLayout.h"
#include "../Buffer/Buffer.h"

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
        std::vector<Vector2> GetUVs() const;
        std::vector<Color> GetColors() const;

        void SetTriangles(std::vector<unsigned short> data);
        void SetPositions(std::vector<Vector3> data);
        void SetNormals(std::vector<Vector3> data);
        void SetUVs(std::vector<Vector2> data);
        void SetColors(std::vector<Color> data);
        void SetPass() const; // 设置当前渲染管线中的顶点索引数据

        void UploadMeshData();
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
}
