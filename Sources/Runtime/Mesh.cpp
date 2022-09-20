#include "Mesh.h"
#include "Vector3.h"
#include "Color.h"
#include "GL.h"
#include<vector>

namespace BDXKEngine {
	ObjectPtr<Mesh> Mesh::Create()
	{
		Mesh mesh = {};
		mesh.vertices = { Vertex{} };
		mesh.triangles = { 0,0,0 };

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

	void Mesh::SetTriangles(std::vector<unsigned short> data)
	{
		size_t size = data.size();
		if (size % 3 != 0)
			throw std::exception("三角形数量有问题");
		if (triangles.size() != size)
		{
			triangles.resize(size);
			ResetTrianglesBuffer();
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
			ResetVerticesBuffer();
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
			ResetVerticesBuffer();
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
			ResetVerticesBuffer();
		}
		for (int index = 0; index < size; index++)
		{
			vertices[index].uv = { data[index].x,1 - data[index].y };
		}
	}
	void Mesh::SetColors(std::vector<Color> data)
	{
		size_t size = data.size();
		if (vertices.size() != size)
		{
			vertices.resize(size);
			ResetVerticesBuffer();
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
		vertexBuffer = Buffer::Create(BufferTarget::Vertex, (int)(vertices.size() * sizeof(Vertex)));
	}
	void Mesh::ResetTrianglesBuffer()
	{
		triangleBuffer = Buffer::Create(BufferTarget::Index, (int)(triangles.size() * sizeof(unsigned short)));
	}

	void Mesh::Export(Exporter& exporter)
	{
		Object::Export(exporter);

		exporter.TransferInt(vertices.size());
		exporter.TransferBytes(vertices.data(), vertices.size() * sizeof(Vertex));
		exporter.TransferInt(triangles.size());
		exporter.TransferBytes(triangles.data(), triangles.size() * sizeof(unsigned short));
	}
	void Mesh::Import(Importer& importer)
	{
		Object::Import(importer);

		int verticesCount = importer.TransferInt();
		vertices.resize(verticesCount);
		importer.TransferBytes(vertices.data(), verticesCount * sizeof(Vertex));

		int trianglesCount = importer.TransferInt();
		triangles.resize(trianglesCount);
		importer.TransferBytes(triangles.data(), trianglesCount * sizeof(unsigned short));
	}
	void Mesh::Awake()
	{
		ResetVerticesBuffer();
		ResetTrianglesBuffer();
	}
}