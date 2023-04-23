#pragma once
#include <rapidjson/document.h>

#include "Core/Editor/Core/Assets/AssetImporter.h"
#include "Core/Runtime/Framework/Actor/GameObject.h"
#include "Render/Runtime/Graphics/Mesh.h"

namespace BDXKEditor
{
    struct ModelMeshData
    {
        std::vector<unsigned short> triangles;
        std::vector<Vector3> positions;
        std::vector<Vector3> normals;
        std::vector<Vector4> tangents;
        std::vector<Vector2> uvs;
        std::vector<Color> colors;

        ObjectPtr<Mesh> GenerateMesh();
    };

    class GlbImporter
    {
    public:
        ObjectPtr<GameObject> ImportAsset(const std::filesystem::path& filePath);

        GlbImporter(const std::function<void(ObjectPtrBase& objectPtr, const FileID& fileID)>& bindObjectRef);

    private:
        unsigned int fileLength = 0;
        unsigned int jsonLength = 0;
        std::string json = {};
        unsigned int bufferLength = 0;
        std::unique_ptr<char[]> buffer = nullptr;
        rapidjson::Document document = rapidjson::Document();
        std::vector<std::vector<ObjectPtr<Mesh>>> meshes;
        std::unordered_set<FileID> fileIDs;

        std::function<void(ObjectPtrBase& objectPtr, const FileID& fileID)> bindObjectRef;

        template <typename T>
        void ImportMeshAttributes(rapidjson::Value& attributes, const char* attributeName, std::vector<T>& dstBuffer, size_t maxCount)
        {
            if (attributes.HasMember(attributeName))
            {
                rapidjson::Value& accessor = document["accessors"][attributes[attributeName].GetUint()];
                rapidjson::Value& bufferView = document["bufferViews"][accessor["bufferView"].GetUint()];
                dstBuffer.resize(bufferView["byteLength"].GetUint() / sizeof(T));
                std::memcpy(dstBuffer.data(), buffer.get() + bufferView["byteOffset"].GetUint(), bufferView["byteLength"].GetUint());
            }
            else dstBuffer.resize(maxCount);
        }
        ObjectPtr<GameObject> ImportGameObject(int nodeIndex, const std::string& rootPath, const ObjectPtr<GameObject>& parent);
        FileID GenerateFileID(const std::string& path);
    };

    class ModelImporter : public AssetImporter
    {
    public:
        ObjectPtrBase Import() override;
    };

    CustomReflection(ModelImporter)
    CustomAssetsImporter(glb, ModelImporter)
}
