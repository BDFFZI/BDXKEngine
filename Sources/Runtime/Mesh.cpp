#include "Mesh.h"
#include "Vector3.h"
#include "Color.h"
#include "GL.h"
#include<vector>

namespace BDXKEngine {
	ObjectPtr<Mesh> Mesh::CreateCube()
	{
		ObjectPtr<Mesh> mesh = ObjectPtr<Mesh>{ new Mesh() };
		mesh->vertices = {
		{{ -0.5f, -0.5f, -0.5f},{},{}, {0,0,0}},
		{{ -0.5f, -0.5f, 0.5f },{},{}, {0,0,1}},
		{{ 0.5f, -0.5f, 0.5f },{},{}, {0,0,1}},
		{{ 0.5f,-0.5f,-0.5f },{},{}, {1,0,0}},
		{{ -0.5f,0.5f,-0.5f },{},{}, {0,1,0}},
		{{ -0.5f,0.5f,0.5f },{},{}, {0,0,1}},
		{{0.5f,0.5f, 0.5f },{},{}, {0,0,1}},
		{{0.5f,0.5f, -0.5f },{},{}, {1,1,0}}
		};
		mesh->triangles = {//前后左右上下
			0,4,7,
			0,7,3,
			2,6,5,
			2,5,1,
			1,5,4,
			1,4,0,
			3,7,6,
			3,6,2,
			4,5,6,
			4,6,7,
			0,3,2,
			0,2,1
		};
		mesh->UploadMeshData();

		return mesh;
	}

	Mesh::Mesh() :Object(L"New Mesh")
	{

	}
	int Mesh::GetVerticesCount()
	{
		return (int)vertices.size();
	}
	int Mesh::GetTrianglesCount()
	{
		return (int)triangles.size();
	}
	std::vector<unsigned int> Mesh::GetTriangles()
	{
		return { triangles };
	}
	std::vector<Vector3> Mesh::GetPositions()
	{
		size_t size = vertices.size();
		std::vector <Vector3> positions{ size };
		for (int index = 0; index < size; index++)
		{
			positions[index] = vertices[index].position;
		}
		return positions;
	}
	std::vector<Vector3> Mesh::GetNormals()
	{
		size_t size = vertices.size();
		std::vector <Vector3> normals{ size };
		for (int index = 0; index < size; index++)
		{
			normals[index] = vertices[index].normal;
		}
		return normals;
	}
	std::vector<Vector2> Mesh::GetUVs()
	{
		size_t size = vertices.size();
		std::vector <Vector2> uvs{ size };
		for (int index = 0; index < size; index++)
		{
			uvs[index] = vertices[index].uv;
		}
		return uvs;
	}
	std::vector<Color> Mesh::GetColors()
	{
		size_t size = vertices.size();
		std::vector <Color> colors{ size };
		for (int index = 0; index < size; index++)
		{
			colors[index] = vertices[index].color;
		}
		return colors;
	}

	void Mesh::SetTriangles(std::vector<UINT32> data)
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
	void Mesh::SetPositions(std::vector<Vector3> data)
	{
		size_t size = data.size();
		vertices.resize(size);
		for (int index = 0; index < size; index++)
		{
			vertices[index].position = data[index];
		}
	}
	void Mesh::SetNormals(std::vector<Vector3> data)
	{
		size_t size = data.size();
		vertices.resize(size);
		for (int index = 0; index < size; index++)
		{
			vertices[index].normal = data[index];
		}
	}
	void Mesh::SetUVs(std::vector<Vector2> data)
	{
		size_t size = data.size();
		vertices.resize(size);
		for (int index = 0; index < size; index++)
		{
			vertices[index].uv = { data[index].x,1 - data[index].y };
		}
	}
	void Mesh::SetColors(std::vector<Color> data)
	{
		size_t size = data.size();
		vertices.resize(size);
		for (int index = 0; index < size; index++)
		{
			vertices[index].color = data[index];
		}
	}

	void Mesh::UploadMeshData()
	{
		GL::CreateBuffer(vertices.data(), (int)(vertices.size() * sizeof(Vertex)), D3D11_BIND_VERTEX_BUFFER, &vertexBuffer.p);
		GL::CreateBuffer(triangles.data(), (int)(triangles.size() * sizeof(int)), D3D11_BIND_INDEX_BUFFER, &triangleBuffer.p);
	}
}