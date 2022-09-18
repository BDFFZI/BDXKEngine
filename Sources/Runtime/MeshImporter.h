#pragma once
#include<string>
#include<vector>
#include<cassert>
#include<fbxsdk.h>
#include"Mesh.h"
#include"Vector3.h"
#include"Color.h"
#include"Debug.h"

namespace BDXKEngine {
	struct StaticMesh {
		std::vector<unsigned int> triangles;
		std::vector<Vector3> positions;
		std::vector<Vector3> normals;
		std::vector<Vector2> uvs;
		std::vector<Color> colors;

		std::wstring ToString()
		{
			std::wstringstream string;

			string << "三角面\n";
			for (int i = 0; i < triangles.size(); i += 3)
			{
				string << "{" << triangles[i + 0] << "," << triangles[i + 1] << "," << triangles[i + 2] << "}\n";
			}
			string << "顶点位置\n";
			for (int i = 0; i < positions.size(); i++)
			{
				string << positions[i].ToString() << "\n";
			}
			string << "顶点法线\n";
			for (int i = 0; i < normals.size(); i++)
			{
				string << normals[i].ToString() << "\n";
			}
			string << "顶点UV\n";
			for (int i = 0; i < uvs.size(); i++)
			{
				string << uvs[i].ToString() << "\n";
			}
			string << "顶点颜色\n";
			for (int i = 0; i < colors.size(); i++)
			{
				string << colors[i].ToString() << "\n";
			}

			return string.str();
		}


		ObjectPtr<Mesh> CreateMesh()
		{
			ObjectPtr<Mesh> mesh = Mesh::Create();
			mesh->SetTriangles(triangles);
			mesh->SetPositions(positions);
			mesh->SetNormals(normals);
			mesh->SetUVs(uvs);
			mesh->SetColors(colors);
			mesh->UploadMeshData();
			return mesh;
		}
	};

	class MeshImporter
	{
	public:
		/// <summary>
		/// 功能目前极其简陋，目前纯粹只读取网格，它的变换参数也不会被导入
		/// 1.要保证fbx文件中只有一个模型且是三角面化的
		/// 2.对应一个点对应多个法线uv的情况需要手动拆分点
		/// </summary>
		/// <param name="fileName"></param>
		/// <returns></returns>
		static StaticMesh ImportFbx(char* fileName)
		{
			StaticMesh stateMesh;

			//初始化FbxManager环境
			FbxManager* manager = FbxManager::Create();
			FbxIOSettings* ioSettings = FbxIOSettings::Create(manager, "FbxIOSettings");
			manager->SetIOSettings(ioSettings);
			//创建导入器
			FbxImporter* importer = FbxImporter::Create(manager, "FbxImporter");
			assert(importer->Initialize(fileName));
			//导入Fbx数据
			FbxScene* scene = FbxScene::Create(manager, "FbxScene");
			importer->Import(scene);
			//解析Fbx数据
			FbxMesh* mesh = scene->GetRootNode()->GetChild(0)->GetMesh();

			int trianglesCount = mesh->GetPolygonVertexCount();
			{//三角面
				stateMesh.triangles.resize(trianglesCount);
				for (int i = 0; i < trianglesCount; i++)
				{
					stateMesh.triangles[i] = mesh->GetPolygonVertex(i / 3, i % 3);
				}
			}
			int vertexCount = mesh->GetControlPointsCount();
			{//顶点位置
				fbxsdk::FbxVector4* positions = mesh->GetControlPoints();
				stateMesh.positions.resize(vertexCount);
				for (int i = 0; i < vertexCount; i++)
				{
					FbxVector4 vector4 = *(positions + i);
					stateMesh.positions[i] = Vector3{ (float)vector4[0],(float)vector4[1],(float)vector4[2] };
				}
			}
			{//顶点法线
				stateMesh.normals.resize(vertexCount);
				for (int i = 0; i < trianglesCount; i++)
				{
					fbxsdk::FbxVector4 normal;
					mesh->GetPolygonVertexNormal(i / 3, i % 3, normal);
					stateMesh.normals[stateMesh.triangles[i]] = { (float)normal[0],(float)normal[1],(float)normal[2] };
				}
			}
			{//顶点uv
				stateMesh.uvs.resize(vertexCount);
				FbxStringList uvNames;
				mesh->GetUVSetNames(uvNames);
				if (uvNames.GetCount() != 0)
				{
					fbxsdk::FbxString uvName = uvNames[0];
					bool isUnmappedUV;
					for (int i = 0; i < trianglesCount; i++)
					{
						FbxVector2 uv;
						mesh->GetPolygonVertexUV(i / 3, i % 3, uvName, uv, isUnmappedUV);
						stateMesh.uvs[stateMesh.triangles[i]] = { (float)uv[0],(float)uv[1] };
					}
				}
			}
			{//顶点颜色
				stateMesh.colors.resize(vertexCount);
				//if (mesh->GetElementVertexColorCount() != 0)
				//{
				//	fbxsdk::FbxGeometryElementVertexColor* vertexColor = mesh->GetElementVertexColor(0);
				//	Debug::Log((String)vertexColor->GetMappingMode());
				//	Debug::Log((String)vertexColor->GetReferenceMode());
				//	//fbxsdk::FbxLayerElementArrayTemplate<fbxsdk::FbxColor> colors = vertexColor->GetDirectArray();
				//	//for (int i = 0; i < vertexCount; i++)
				//	//{
				//	//	stateMesh.colors[i] = {
				//	//		(float)colors.GetAt(i).mRed,
				//	//		(float)colors.GetAt(i).mGreen,
				//	//		(float)colors.GetAt(i).mBlue,
				//	//		(float)colors.GetAt(i).mAlpha
				//	//	};
				//	//}
				//}
			}

			//我看官方示例就手动了这两个
			importer->Destroy();
			manager->Destroy();

			return stateMesh;
		}
	};
}
