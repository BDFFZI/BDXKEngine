#include "ModelImporter.h"
#include <fstream>
#include <rapidjson/document.h>

#include "RenderAssets.h"
#include "Core/Runtime/Framework/Actor/GameObject.h"
#include "Render/Editor/EditorMenu/RenderAssetsMenu.h"
#include "Render/Runtime/Graphics/Mesh.h"
#include "Render/Runtime/Renderer/MeshRenderer.h"

namespace BDXKEditor
{
    ObjectPtr<Mesh> ModelMeshData::GenerateMesh()
    {
        //右手坐标系转左手坐标系(右手坐标系导出时默认y向上，-z向前，此时和左手坐标系的差别就是x反了)
        const size_t trianglesCount = triangles.size();
        for (size_t i = 0; i < trianglesCount; i += 3)
        {
            const unsigned short temp = triangles[i + 0];
            triangles[i + 0] = triangles[i + 2];
            triangles[i + 2] = temp;
        }
        for (auto& position : positions)
            position = {-position.x, position.y, position.z};
        for (auto& normal : normals)
            normal = {-normal.x, normal.y, normal.z};
        for (auto& tangent : tangents)
            tangent = {-tangent.x, tangent.y, tangent.z, tangent.w};

        ObjectPtr<Mesh> mesh = Mesh::Create();
        mesh->SetTriangles(triangles);
        mesh->SetPositions(positions);
        mesh->SetNormals(normals);
        mesh->SetTangents(tangents);
        mesh->SetUVs(uvs);
        mesh->SetColors(colors);
        mesh->UpdateGL();
        return mesh;
    }

    GlbImporter::GlbImporter(const std::function<void(ObjectPtrBase& objectPtr, const FileID& fileID)>& bindObjectRef)
    {
        this->bindObjectRef = bindObjectRef;
    }
    ObjectPtr<GameObject> GlbImporter::ImportAsset(const std::filesystem::path& filePath)
    {
        //获取基本数据
        std::ifstream fileData = std::ifstream(filePath, std::ios_base::binary);
        fileData.ignore(8);
        fileData.read(reinterpret_cast<char*>(&fileLength), 4);
        fileData.read(reinterpret_cast<char*>(&jsonLength), 4);
        fileData.ignore(4);
        json.resize(jsonLength);
        fileData.read(json.data(), jsonLength);
        fileData.read(reinterpret_cast<char*>(&bufferLength), 4);
        fileData.ignore(4);
        fileData.read((buffer = std::unique_ptr<char[]>(new char[bufferLength])).get(), bufferLength);
        document.Parse(json.c_str(), jsonLength);

        //导入网格
        const auto meshAssets = document["meshes"].GetArray();
        for (rapidjson::SizeType meshIndex = 0; meshIndex < meshAssets.Size(); meshIndex++)
        {
            rapidjson::Value& meshAsset = meshAssets[meshIndex];
            meshes.emplace_back();

            const auto primitives = meshAsset["primitives"].GetArray();
            for (rapidjson::SizeType primitiveIndex = 0; primitiveIndex < primitives.Size(); primitiveIndex++)
            {
                rapidjson::Value& primitive = primitives[primitiveIndex];
                rapidjson::Value& attributes = primitive["attributes"];
                //解析网格数据
                ModelMeshData stateMesh = {};
                ImportMeshAttributes(primitive, "indices", stateMesh.triangles, -1); //索引数据
                ImportMeshAttributes(attributes, "POSITION", stateMesh.positions, -1); //位置
                ImportMeshAttributes(attributes, "NORMAL", stateMesh.normals, stateMesh.positions.size()); //法线
                ImportMeshAttributes(attributes, "TANGENT", stateMesh.tangents, stateMesh.positions.size()); //切线
                ImportMeshAttributes(attributes, "TEXCOORD_0", stateMesh.uvs, stateMesh.positions.size()); //UV
                ImportMeshAttributes(attributes, "COLOR_0", stateMesh.colors, stateMesh.positions.size()); //颜色
                //生成网格
                ObjectPtr<Mesh> mesh = stateMesh.GenerateMesh();
                mesh->SetName(meshAsset["name"].GetString());
                bindObjectRef(mesh, GenerateFileID("Meshes/" + mesh->GetName()));
                meshes[meshIndex].push_back(mesh);
            }
        }

        //解析场景结构
        const auto rootNode = document["scenes"][0]["nodes"].GetArray();
        if (rootNode.Size() > 1)
        {
            ObjectPtr<GameObject> scene = GameObject::Create(filePath.filename().string(), nullptr, nullptr, false);
            bindObjectRef(scene, GenerateFileID(scene->GetName()));
            std::string currentPath = scene->GetName() + "/";
            for (rapidjson::SizeType i = 0; i < rootNode.Size(); i++)
                ImportGameObject(rootNode[i].GetInt(), currentPath, scene);
            return scene;
        }

        return ImportGameObject(rootNode[0].GetInt(), filePath.filename().string(), nullptr);
    }
    ObjectPtr<GameObject> GlbImporter::ImportGameObject(const int nodeIndex, const std::string& rootPath, const ObjectPtr<GameObject>& parent)
    {
        rapidjson::Value& node = document["nodes"][nodeIndex];

        ObjectPtr<GameObject> gameObject = GameObject::Create(node["name"].GetString(), parent, nullptr, false);
        std::string currentPath = rootPath + gameObject->GetName();
        bindObjectRef(gameObject, GenerateFileID(rootPath + gameObject->GetName()));

        //导入变换
        if (node.HasMember("translation"))
            gameObject->SetPosition({node["translation"][0].GetFloat(), node["translation"][1].GetFloat(), node["translation"][2].GetFloat()});
        if (node.HasMember("rotation"))
            gameObject->SetEulerAngles(
                Quaternion{
                    node["rotation"][0].GetFloat(), node["rotation"][1].GetFloat(), node["rotation"][2].GetFloat(), node["rotation"][3].GetFloat()
                }.ToEulerAngles()
            );
        if (node.HasMember("scale"))
            gameObject->SetScale({node["scale"][0].GetFloat(), node["scale"][1].GetFloat(), node["scale"][2].GetFloat()});
        //导入子类
        currentPath += "/";
        if (node.HasMember("children"))
        {
            const auto child = node["children"].GetArray();
            for (rapidjson::SizeType i = 0; i < child.Size(); i++)
                ImportGameObject(child[i].GetInt(), currentPath, gameObject);
        }
        //导入网格
        if (node.HasMember("mesh"))
        {
            const std::vector<ObjectPtr<Mesh>>& submeshes = meshes[node["mesh"].GetInt()];
            for (const auto& submesh : submeshes)
            {
                ObjectPtr<MeshRenderer> meshRenderer = Component::Create<MeshRenderer>(gameObject, false);
                bindObjectRef(meshRenderer, GenerateFileID(currentPath + "Component:MeshRenderer:" + submesh->GetName()));
                meshRenderer->SetMesh(submesh);
            }
        }

        return gameObject;
    }

    FileID GlbImporter::GenerateFileID(const std::string& path)
    {
        constexpr std::hash<std::string> hash;
        FileID fileID = hash(path);
        while (fileIDs.contains(fileID))
            fileID++;
        fileIDs.insert(fileID);
        return fileID;
    }

    ObjectPtrBase ModelImporter::Import()
    {
        GlbImporter glbImporter = {[this](ObjectPtrBase& objectPtr, const FileID& fileID) { BindObjectRef(objectPtr, fileID); }};
        return static_cast<ObjectPtrBase>(glbImporter.ImportAsset(GetTargetPath()));
    }
}
