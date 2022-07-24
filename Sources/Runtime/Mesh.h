#pragma once
#include <exception>
#include <vector>
#include "ObjectPtr.h"
#include "GLLayout.h"
#include "GL.h"

namespace BDXKEngine {
	class Mesh :public Object, GL
	{
		friend GL;
	public:
		static ObjectPtr<Mesh> CreateCube();

		Mesh();

		int GetVerticesCount();
		int GetTrianglesCount();
		std::vector <unsigned int> GetTriangles();
		std::vector <Vector3> GetPositions();
		std::vector <Vector3> GetNormals();
		std::vector<Vector2> GetUVs();
		std::vector<Color> GetColors();

		void SetTriangles(std::vector <UINT32> data);
		void SetPositions(std::vector <Vector3> data);
		void SetNormals(std::vector <Vector3> data);
		void SetUVs(std::vector<Vector2> data);
		void SetColors(std::vector<Color> data);

		void UploadMeshData();
	private:
		std::vector<Vertex> vertices;
		std::vector<UINT32> triangles;

		CComPtr<ID3D11Buffer> vertexBuffer;
		CComPtr<ID3D11Buffer> triangleBuffer;
	};
}

