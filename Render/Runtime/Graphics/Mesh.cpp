#include "Mesh.h"

namespace BDXKEngine
{
    ObjectPtr<Mesh> Mesh::Create()
    {
        ObjectPtr mesh = new Mesh{};
        mesh->vertices = {Vertex{}};
        mesh->triangles = {0, 0, 0};
        AwakeObject(mesh);

        return mesh;
    }
    int Mesh::GetVerticesCount() const
    {
        return static_cast<int>(vertices.size());
    }
    int Mesh::GetTrianglesCount() const
    {
        return static_cast<int>(triangles.size());
    }
    std::vector<unsigned short> Mesh::GetTriangles()
    {
        return {triangles};
    }
    std::vector<Vector3> Mesh::GetPositions() const
    {
        const size_t size = vertices.size();
        std::vector<Vector3> positions{size};
        for (size_t index = 0; index < size; index++)
        {
            positions[index] = vertices[index].position;
        }
        return positions;
    }
    std::vector<Vector3> Mesh::GetNormals() const
    {
        const size_t size = vertices.size();
        std::vector<Vector3> normals{size};
        for (size_t index = 0; index < size; index++)
        {
            normals[index] = vertices[index].normal;
        }
        return normals;
    }
    std::vector<Vector4> Mesh::GetTangents() const
    {
        const size_t size = vertices.size();
        std::vector<Vector4> tangents{size};
        for (size_t index = 0; index < size; index++)
        {
            tangents[index] = vertices[index].tangent;
        }
        return tangents;
    }
    std::vector<Vector2> Mesh::GetUVs() const
    {
        const size_t size = vertices.size();
        std::vector<Vector2> uvs{size};
        for (size_t index = 0; index < size; index++)
        {
            uvs[index] = vertices[index].uv;
        }
        return uvs;
    }
    std::vector<Color> Mesh::GetColors() const
    {
        const size_t size = vertices.size();
        std::vector<Color> colors{size};
        for (size_t index = 0; index < size; index++)
        {
            colors[index] = vertices[index].color;
        }
        return colors;
    }
    std::vector<Vertex> Mesh::GetVertices() const
    {
        return vertices;
    }

    void Mesh::SetTriangles(const std::vector<unsigned short>& data)
    {
        const size_t size = data.size();
        if (size % 3 != 0)
            throw std::exception("三角形数量有问题");
        if (triangles.size() != size)
        {
            triangles.resize(size);
            ResetTrianglesBuffer();
        }
        for (size_t index = 0; index < size; index++)
        {
            triangles[index] = data[index];
        }
    }
    void Mesh::SetPositions(const std::vector<Vector3>& data)
    {
        const size_t size = data.size();
        if (vertices.size() != size)
        {
            vertices.resize(size);
            ResetVerticesBuffer();
        }
        for (size_t index = 0; index < size; index++)
        {
            vertices[index].position = data[index];
        }
    }
    void Mesh::SetNormals(const std::vector<Vector3>& data)
    {
        const size_t size = data.size();
        if (vertices.size() != size)
        {
            vertices.resize(size);
            ResetVerticesBuffer();
        }
        for (size_t index = 0; index < size; index++)
        {
            vertices[index].normal = data[index];
        }
    }
    void Mesh::SetTangents(const std::vector<Vector4>& data)
    {
        const size_t size = data.size();
        if (vertices.size() != size)
        {
            vertices.resize(size);
            ResetVerticesBuffer();
        }
        for (size_t index = 0; index < size; index++)
        {
            vertices[index].tangent = data[index];
        }
    }
    void Mesh::SetUVs(const std::vector<Vector2>& data)
    {
        const size_t size = data.size();
        if (vertices.size() != size)
        {
            vertices.resize(size);
            ResetVerticesBuffer();
        }
        for (size_t index = 0; index < size; index++)
        {
            vertices[index].uv = {data[index].x, 1 - data[index].y};
        }
    }
    void Mesh::SetColors(const std::vector<Color>& data)
    {
        const size_t size = data.size();
        if (vertices.size() != size)
        {
            vertices.resize(size);
            ResetVerticesBuffer();
        }
        for (size_t index = 0; index < size; index++)
        {
            vertices[index].color = data[index];
        }
    }
    void Mesh::SetVertices(const std::vector<Vertex>& data)
    {
        const size_t size = data.size();
        if (vertices.size() != size)
            ResetVerticesBuffer();
        vertices = data;
    }

    void Mesh::UpdateGL() const
    {
        vertexBuffer->SetData(vertices.data());
        triangleBuffer->SetData(triangles.data());
    }
    void Mesh::UploadRP() const
    {
        const auto context = GL::GetDeviceContext();
        const auto vertexGLBuffer = vertexBuffer->GetGLBuffer();
        const auto triangleGLBuffer = triangleBuffer->GetGLBuffer();

        //绑定顶点数据
        constexpr unsigned int stride = sizeof(Vertex);
        constexpr unsigned int offset = 0;
        context->IASetVertexBuffers(0, 1, &vertexGLBuffer.p, &stride, &offset);
        //绑定索引数据
        context->IASetIndexBuffer(triangleGLBuffer.p, DXGI_FORMAT_R16_UINT, 0);
    }

    void Mesh::ResetVerticesBuffer()
    {
        vertexBuffer = Buffer::Create(BufferTarget::Vertex, static_cast<int>(vertices.size() * sizeof(Vertex)));
    }
    void Mesh::ResetTrianglesBuffer()
    {
        triangleBuffer = Buffer::Create(BufferTarget::Index, static_cast<int>(triangles.size() * sizeof(unsigned short)));
    }

    void Mesh::Transfer(Transferer& transferer)
    {
        Object::Transfer(transferer);

        TransferFieldInfoOf(vertices, std::vector<char>);
        TransferFieldInfoOf(triangles, std::vector<char>);
    }
    void Mesh::Awake()
    {
        Object::Awake();

        ResetVerticesBuffer();
        ResetTrianglesBuffer();
        UpdateGL();
    }
}
