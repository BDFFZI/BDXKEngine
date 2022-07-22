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

	Mesh::Mesh(StaticMesh staticMesh) : Mesh()
	{
		SetTriangles(staticMesh.triangles);
		SetPositions(staticMesh.positions);
		SetNormals(staticMesh.normals);
		SetUVs(staticMesh.uvs);
		SetColors(staticMesh.colors);

		UploadMeshData();
	}
}