#include "Mesh.h"
#include "BDXKEngine/Base/Vector/Vector3.h"
#include "BDXKEngine/Base/Color.h"
#include "BDXKEngine/Platform/GL/GL.h"
#include<vector>

namespace BDXKEngine
{
    ObjectPtr<Mesh> Mesh::Create()
    {
        Mesh mesh = {};
        mesh.vertices = {Vertex{}};
        mesh.triangles = {0, 0, 0};

        return Instantiate<Mesh>(&mesh);
    }
    int Mesh::GetVerticesCount()
    {
        return (int)vertices.size();
    }
    int Mesh::GetTrianglesCount()
    {
        return (int)triangles.size();
    }
    std::vector<unsigned short> Mesh::GetTriangles()
    {
        return {triangles};
    }
    std::vector<Vector3> Mesh::GetPositions()
    {
        size_t size = vertices.size();
        std::vector<Vector3> positions{size};
        for (int index = 0; index < size; index++)
        {
            positions[index] = vertices[index].position;
        }
        return positions;
    }
    std::vector<Vector3> Mesh::GetNormals()
    {
        size_t size = vertices.size();
        std::vector<Vector3> normals{size};
        for (int index = 0; index < size; index++)
        {
            normals[index] = vertices[index].normal;
        }
        return normals;
    }
    std::vector<Vector2> Mesh::GetUVs()
    {
        size_t size = vertices.size();
        std::vector<Vector2> uvs{size};
        for (int index = 0; index < size; index++)
        {
            uvs[index] = vertices[index].uv;
        }
        return uvs;
    }
    std::vector<Color> Mesh::GetColors()
    {
        size_t size = vertices.size();
        std::vector<Color> colors{size};
        for (int index = 0; index < size; index++)
        {
            colors[index] = vertices[index].color;
        }
        return colors;
    }

    void Mesh::SetTriangles(std::vector<unsigned short> data)
    {
        size_t size = data.size();
        if (size % 3 != 0)
            throw std::exception("三角形数量有问题");
        if (triangles.size() != size)
        {
            triangles.resize(size);
            if (GetIsRunning())ResetTrianglesBuffer();
        }
        for (int index = 0; index < size; index++)
        {
            triangles[index] = data[index];
        }
    }
    void Mesh::SetPositions(std::vector<Vector3> data)
    {
        size_t size = data.size();
        if (vertices.size() != size)
        {
            vertices.resize(size);
            if (GetIsRunning())ResetVerticesBuffer();
        }
        for (int index = 0; index < size; index++)
        {
            vertices[index].position = data[index];
        }
    }
    void Mesh::SetNormals(std::vector<Vector3> data)
    {
        size_t size = data.size();
        if (vertices.size() != size)
        {
            vertices.resize(size);
            if (GetIsRunning())ResetVerticesBuffer();
        }
        for (int index = 0; index < size; index++)
        {
            vertices[index].normal = data[index];
        }
    }
    void Mesh::SetUVs(std::vector<Vector2> data)
    {
        size_t size = data.size();
        if (vertices.size() != size)
        {
            vertices.resize(size);
            if (GetIsRunning())ResetVerticesBuffer();
        }
        for (int index = 0; index < size; index++)
        {
            vertices[index].uv = {data[index].x, 1 - data[index].y};
        }
    }
    void Mesh::SetColors(std::vector<Color> data)
    {
        size_t size = data.size();
        if (vertices.size() != size)
        {
            vertices.resize(size);
            if (GetIsRunning())ResetVerticesBuffer();
        }
        for (int index = 0; index < size; index++)
        {
            vertices[index].color = data[index];
        }
    }

    void Mesh::UploadMeshData()
    {
        vertexBuffer->SetData(reinterpret_cast<char*>(vertices.data()));
        triangleBuffer->SetData(reinterpret_cast<char*>(triangles.data()));
    }
    void Mesh::ResetVerticesBuffer()
    {
        vertexBuffer = Buffer::Create(BufferTarget::Vertex, static_cast<int>(vertices.size() * sizeof(Vertex)));
    }
    void Mesh::ResetTrianglesBuffer()
    {
        triangleBuffer = Buffer::Create(BufferTarget::Index, static_cast<int>(triangles.size() * sizeof(unsigned short)));
    }

    void Mesh::Transfer(Transferrer& transferrer)
    {
        Object::Transfer(transferrer);

        int verticesCount = static_cast<int>(vertices.size());
        int trianglesCount = static_cast<int>(triangles.size());
        transferrer.TransferField(nameof(verticesCount), verticesCount);
        transferrer.TransferField(nameof(trianglesCount), trianglesCount);

        if (transferrer.GetTransferDirection() == TransferDirection::Input)
        {
            vertices.resize(verticesCount);
            triangles.resize(trianglesCount);
        }
        transferrer.TransferField(
            "verticesData",
            reinterpret_cast<char*>(vertices.data()),
            static_cast<int>(verticesCount * sizeof(Vertex))
        );
        transferrer.TransferField(
            "trianglesData",
            reinterpret_cast<char*>(triangles.data()),
            static_cast<int>(trianglesCount * sizeof(unsigned short))
        );
    }
    void Mesh::PreAwake()
    {
        ResetVerticesBuffer();
        ResetTrianglesBuffer();
    }
}
