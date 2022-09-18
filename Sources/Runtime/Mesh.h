#pragma once
#include <exception>
#include <vector>
#include "ObjectPtr.h"
#include "GLLayout.h"
#include "GL.h"
#include "Buffer.h"

namespace BDXKEngine {
	class Mesh :public Object, GL
	{
		friend GL;
	public:
		static ObjectPtr<Mesh> Create();

		int GetVerticesCount();
		int GetTrianglesCount();
		std::vector <unsigned int> GetTriangles();
		std::vector <Vector3> GetPositions();
		std::vector <Vector3> GetNormals();
		std::vector<Vector2> GetUVs();
		std::vector<Color> GetColors();

		void SetTriangles(std::vector <unsigned int> data);
		void SetPositions(std::vector <Vector3> data);
		void SetNormals(std::vector <Vector3> data);
		void SetUVs(std::vector<Vector2> data);
		void SetColors(std::vector<Color> data);

		void UploadMeshData();
	private:
		std::vector<Vertex> vertices = {};
		std::vector<unsigned int> triangles = {};

		ObjectPtr<Buffer> vertexBuffer;
		ObjectPtr<Buffer> triangleBuffer;

		void ResetVerticesBuffer();
		void ResetTrianglesBuffer();

		void Export(Exporter& exporter) override;
		void Import(Importer& importer) override;
		void Awake()override;
	};
}

