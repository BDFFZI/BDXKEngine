#pragma once
#include<exception>
#include<vector>
#include "GLLayout.h"

class MeshEditor;
class Mesh
{
	friend MeshEditor;
public:
	Mesh();

	int GetVerticesCount()
	{
		return (int)vertices.size();
	}
	int GetTrianglesCount()
	{
		return (int)triangles.size();
	}
	std::vector <UINT32> GetTriangles()
	{
		return { triangles };
	}
	std::vector <Vector3> GetPositions()
	{
		size_t size = vertices.size();
		std::vector <Vector3> positions{ size };
		for (int index = 0; index < size; index++)
		{
			positions[index] = vertices[index].position;
		}
	}
	std::vector <Vector3> GetNormals()
	{
		size_t size = vertices.size();
		std::vector <Vector3> normals{ size };
		for (int index = 0; index < size; index++)
		{
			normals[index] = vertices[index].normal;
		}
	}
	void GetColors(std::vector <Color> data)
	{
		size_t size = data.size();
		vertices.resize(size);
		for (int index = 0; index < size; index++)
		{
			vertices[index].color = data[index];
		}
	}

	void SetTriangles(std::vector <UINT32> data)
	{
		size_t size = data.size();
		if (size % 3 != 0)
			throw std::exception("三角形数量有问题");

		triangles.resize(size);
		for (int index = 0; index < size; index++)
		{
			triangles[index] = data[index];
		}
	}
	void SetPositions(std::vector <Vector3> data)
	{
		size_t size = data.size();
		vertices.resize(size);
		for (int index = 0; index < size; index++)
		{
			vertices[index].position = data[index];
		}
	}
	void SetNormals(std::vector <Vector3> data)
	{
		size_t size = data.size();
		vertices.resize(size);
		for (int index = 0; index < size; index++)
		{
			vertices[index].normal = data[index];
		}
	}
	std::vector <Color> SetColors()
	{
		size_t size = vertices.size();
		std::vector <Color> colors{ size };
		for (int index = 0; index < size; index++)
		{
			colors[index] = vertices[index].color;
		}
	}

	void UploadMeshData()
	{
		GL::CreateVertexBuffer(vertices.data(), (int)(vertices.size() * sizeof(Vertex)), &vertexBuffer);
		GL::CreateIndexBuffer(triangles.data(), (int)(triangles.size() * sizeof(int)), &triangleBuffer);
	}
private:
	std::vector<Vertex> vertices;
	std::vector<UINT32> triangles;

	CComPtr<ID3D11Buffer> vertexBuffer;
	CComPtr<ID3D11Buffer> triangleBuffer;
};

class MeshEditor {
protected:
	static CComPtr<ID3D11Buffer> GetVertexBuffer(Mesh* mesh)
	{
		return mesh->vertexBuffer;
	}

	static CComPtr<ID3D11Buffer> GetTriangleBuffer(Mesh* mesh)
	{
		return mesh->triangleBuffer;
	}
};

