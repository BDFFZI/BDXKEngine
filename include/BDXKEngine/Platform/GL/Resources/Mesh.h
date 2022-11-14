#pragma once
#include <vector>
#include "BDXKEngine/Base/Object/ObjectPtr.h"
#include "GLLayout.h"
#include "BDXKEngine/Platform/GL/GL.h"
#include "Buffer.h"

namespace BDXKEngine {
	class Mesh :public Object, GL
	{
		friend GL;
	public:
		static ObjectPtr<Mesh> Create();

		int GetVerticesCount();
		int GetTrianglesCount();
		std::vector <unsigned short> GetTriangles();
		std::vector <Vector3> GetPositions();
		std::vector <Vector3> GetNormals();
		std::vector<Vector2> GetUVs();
		std::vector<Color> GetColors();

		void SetTriangles(std::vector <unsigned short> data);
		void SetPositions(std::vector <Vector3> data);
		void SetNormals(std::vector <Vector3> data);
		void SetUVs(std::vector<Vector2> data);
		void SetColors(std::vector<Color> data);

		void UploadMeshData();
	private:
		std::vector<Vertex> vertices = {};
		std::vector<unsigned short> triangles = {};

		ObjectPtr<Buffer> vertexBuffer;
		ObjectPtr<Buffer> triangleBuffer;

		void ResetVerticesBuffer();
		void ResetTrianglesBuffer();

		void Transfer(transferer& transferer) override;
		void MarkAwake()override;
	};
}

