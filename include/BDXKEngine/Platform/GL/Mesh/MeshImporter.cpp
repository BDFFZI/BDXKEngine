#include "MeshImporter.h"
#include<fstream>
#include<rapidjson/document.h>
#include<rapidjson/writer.h>

namespace BDXKEngine
{
    struct MeshData
    {
        std::vector<unsigned short> triangles;
        std::vector<Vector3> positions;
        std::vector<Vector3> normals;
        std::vector<Vector2> uvs;
        std::vector<Color> colors;

        ObjectPtr<Mesh> ToMesh() const
        {
            ObjectPtr<Mesh> mesh = Mesh::Create();
            mesh->SetTriangles(triangles);
            mesh->SetPositions(positions);
            mesh->SetNormals(normals);
            mesh->SetUVs(uvs);
            mesh->SetColors(colors);
            mesh->UpdataMeshData();
            return mesh;
        }
    };

    MeshData ImportGlb(const std::string& fileName)
    {
        unsigned int fileLength = 0;
        unsigned int jsonLength = 0;
        std::string json = {};
        unsigned int bufferLength = 0;
        std::unique_ptr<char[]> buffer = nullptr;

        std::ifstream fileData = std::ifstream(fileName, std::ios_base::binary);
        fileData.ignore(8);
        fileData.read(reinterpret_cast<char*>(&fileLength), 4);
        fileData.read(reinterpret_cast<char*>(&jsonLength), 4);
        fileData.ignore(4);
        json.resize(jsonLength);
        fileData.read(json.data(), jsonLength);
        fileData.read(reinterpret_cast<char*>(&bufferLength), 4);
        fileData.ignore(4);
        fileData.read((buffer = std::unique_ptr<char[]>(new char[bufferLength])).get(), bufferLength);

        rapidjson::Document document = rapidjson::Document();
        document.Parse(json.c_str(), jsonLength);

        rapidjson::Value& positionsAccessor = document["accessors"][document["meshes"][0]["primitives"][0]["attributes"]["POSITION"].
            GetUint()];
        rapidjson::Value& normalsAccessor = document["accessors"][document["meshes"][0]["primitives"][0]["attributes"]["NORMAL"].GetUint()];
        rapidjson::Value& uvsAccessor = document["accessors"][document["meshes"][0]["primitives"][0]["attributes"]["TEXCOORD_0"].GetUint()];
        rapidjson::Value& trianglesAccessor = document["accessors"][document["meshes"][0]["primitives"][0]["indices"].GetUint()];

        rapidjson::Value& positionsBufferView = document["bufferViews"][positionsAccessor["bufferView"].GetUint()];
        rapidjson::Value& normalsBufferView = document["bufferViews"][normalsAccessor["bufferView"].GetUint()];
        rapidjson::Value& uvsBufferView = document["bufferViews"][uvsAccessor["bufferView"].GetUint()];
        rapidjson::Value& trianglesBufferView = document["bufferViews"][trianglesAccessor["bufferView"].GetUint()];

        MeshData stateMesh;

        stateMesh.positions.resize(positionsBufferView["byteLength"].GetUint() / sizeof(Vector3));
        stateMesh.normals.resize(normalsBufferView["byteLength"].GetUint() / sizeof(Vector3));
        stateMesh.uvs.resize(uvsBufferView["byteLength"].GetUint() / sizeof(Vector2));
        stateMesh.triangles.resize(trianglesBufferView["byteLength"].GetUint() / sizeof(unsigned short));
        stateMesh.colors.resize(positionsAccessor["count"].GetUint());

        std::memcpy(stateMesh.positions.data(), buffer.get() + positionsBufferView["byteOffset"].GetUint(),
                    positionsBufferView["byteLength"].GetUint());
        std::memcpy(stateMesh.normals.data(), buffer.get() + normalsBufferView["byteOffset"].GetUint(),
                    normalsBufferView["byteLength"].GetUint());
        std::memcpy(stateMesh.uvs.data(), buffer.get() + uvsBufferView["byteOffset"].GetUint(), uvsBufferView["byteLength"].GetUint());
        std::memcpy(stateMesh.triangles.data(), buffer.get() + trianglesBufferView["byteOffset"].GetUint(),
                    trianglesBufferView["byteLength"].GetUint());

        return stateMesh;
    }

    ObjectPtr<Mesh> MeshImporter::Import(const std::string& fileName)
    {
        return ImportGlb(fileName).ToMesh();
    }
}
